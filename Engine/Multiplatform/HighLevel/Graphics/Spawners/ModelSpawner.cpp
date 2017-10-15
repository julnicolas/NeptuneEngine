#include "Graphics/Spawners/ModelSpawner.h"
#include "Graphics/VAOView.h"
#include "Graphics/ElementView.h"

// To improve performance, compile Assimp in C
// It will enable direct buffer copies
#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"

#include <fstream>

#include "Debug/NeptuneDebug.h"

#include <stack>

using namespace Neptune;

struct Node
{
	aiNode* m_node;
	u32		m_childIndex;
};

void ModelSpawner::PostFixDepthSearch(const aiScene* _scene, aiNode* _root)
{
	NEP_ASSERT( _root  != nullptr ); // Error invalid pointer
	NEP_ASSERT( _scene != nullptr ); // Error invalid pointer
	
	std::stack<Node, std::vector<Node>> stack;
	Node node;
	node.m_node			= _root;
	node.m_childIndex	= 0;

	do 
	{
		u32 num_children = node.m_node->mNumChildren;
		if ( num_children != 0 && node.m_childIndex < num_children )
		{
			node.m_childIndex++;
			stack.push(node);

			node.m_node = node.m_node->mChildren[node.m_childIndex-1];
			node.m_childIndex = 0;
		}
		else
		{
			ProcessMeshes( _scene, node.m_node );
			
			if ( !stack.empty() ) // Get parent
			{
				node = stack.top();
				stack.pop();
			}
			else // we processed the root
				node.m_node = nullptr;
		}
	} while ( node.m_node != nullptr ); // Have we processed the whole tree?
}

void ModelSpawner::ProcessMeshes(const aiScene* _scene, aiNode* _node)
{
	NEP_ASSERT( _scene != nullptr ); // Error invalid pointer
	NEP_ASSERT( _node  != nullptr ); // Error, invalid pointer
	
	u32 num_meshes = _node->mNumMeshes;
	for ( u32 i = 0; i < num_meshes; i++ )
	{
		u32 mesh_index = _node->mMeshes[i];
		fillMeshData( _scene->mMeshes[mesh_index] );
	}
}

void ModelSpawner::fillMeshData(aiMesh* _mesh)
{
	// check rendering primitive
	// just triangles are supported at the moment
	NEP_ASSERT_ERR_MSG( !(_mesh->mPrimitiveTypes & aiPrimitiveType_POINT) &&
						!(_mesh->mPrimitiveTypes & aiPrimitiveType_LINE)  &&
						!(_mesh->mPrimitiveTypes & aiPrimitiveType_POLYGON), "Only triangles are supported.");

	NEP_ASSERT_ERR_MSG((_mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) != 0, "Error, no drawing primitive has been provided. Check your model file.");

	u32 num_faces			= _mesh->mNumFaces;		// Number of primitives present in the mesh (triangles, lines, points)
	u32 num_vertices		= _mesh->mNumVertices;
	m_nbVerticesToRender	+= num_faces * 3;		// A face is made up of 3 vertices

	// Populate the vertex buffer
	for (u32 i = 0; i < num_vertices; i++)
	{
		m_vertices.push_back(_mesh->mVertices[i].x);
		m_vertices.push_back(_mesh->mVertices[i].y);
		m_vertices.push_back(_mesh->mVertices[i].z);
	}

	// Populate the index buffer
	for (u32 face_index = 0; face_index < num_faces; face_index++)
	{
		// The model is broken down into faces; set of vertices which 
		// represent a geometrical surface. A triangle if mesh->mPrimitiveTypes
		// is aiPrimitiveType_TRIANGLE.
		// Each vertex is referenced by an index (position in mesh->mVertices).
		u32  num_indices	= _mesh->mFaces[face_index].mNumIndices;
		u32* indices		= _mesh->mFaces[face_index].mIndices;

		for (u32 i = 0; i < num_indices; i++)
			m_vertexIndices.push_back( indices[i] );
	}

	// Populate the color buffer
	if ( *(_mesh->mColors) != nullptr )
	{
		Color c;

		// At the moment we support only one color set per model
		const u8 nb_color_sets = 1; // AI_MAX_NUMBER_OF_COLOR_SETS
		for (u8 color_set_index = 0; color_set_index < nb_color_sets; color_set_index++)
		{
			for (u32 i = 0; i < num_vertices; i++)
			{
				c.r = _mesh->mColors[color_set_index][i].r;
				c.g = _mesh->mColors[color_set_index][i].g;
				c.b = _mesh->mColors[color_set_index][i].b;
				c.a = _mesh->mColors[color_set_index][i].a;

				m_colors.push_back(c);
			}
		}
	}

	// Populate the normal buffer
	if ( _mesh->mNormals != nullptr )
	{
		for ( u32 i = 0; i < num_vertices; i++ )
		{
			m_normals.push_back( _mesh->mNormals[i].x );
			m_normals.push_back( _mesh->mNormals[i].y );
			m_normals.push_back( _mesh->mNormals[i].z );
		}
	}

	// Populate the texture coordinates buffer
	/*if ( mesh->mTextureCoords != nullptr )
	{
		// AT the moment only one set of texture coordinate is supported per vertex
		const u8 nb_set_tex_coords = 1; // AI_MAX_NUMBER_OF_TEXTURECOORDS

		for (u8 tex_coord_set = 0; tex_coord_set < nb_set_tex_coords; tex_coord_set++)
		{
			for ( u32 i = 0; i < m_nbVerticesToRender; i++ )
			{
				m_2DTexCoords.push_back( mesh->mTextureCoords[tex_coord_set][i].x ); // u component
				m_2DTexCoords.push_back( mesh->mTextureCoords[tex_coord_set][i].y ); // v component
				// m_2DTexCoords.push_back( mesh->mTextureCoords[tex_coord_set][i].z ); // w component
			}
		}
	}*/
}

ModelSpawner::ModelSpawner(GraphicsProgram* _pgm, const char* _modelPath):
	ViewSpawner(_pgm),
	m_nbVerticesToRender(0), 
	m_drawingPrimitive(Renderer::DrawingPrimitive::TRIANGLES)
{
	// L O A D   M O D E L
	NEP_ASSERT(_modelPath != nullptr); // Error: Invalid file name

	// Create an instance of the Importer class
    Assimp::Importer importer;

    //check if file exists
    std::ifstream fin(_modelPath);
	NEP_ASSERT(!fin.fail()); // Error file doesn't exist

	// Load mesh
	const aiScene* scene = nullptr; 
    scene = importer.ReadFile( _modelPath, aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast);
	NEP_ASSERT(scene != nullptr); // Error Import failed
	
	NEP_ASSERT_ERR_MSG(!(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE), "Error in file content... something is wrong with the data.");

	// Check content
	NEP_ASSERT(scene->mNumMeshes > 0); // Error the file doesn't define a mesh
	//NEP_ASSERT_ERR_MSG(scene->mNumMeshes == 1, "Error the file contains several meshes. Use a different importer.");

	// P O P U L A T E   T H E   B U F F E R S
    
	u32 num_meshses	= scene->mNumMeshes;
	aiNode* node = scene->mRootNode;
	
		
	/*u32 mesh_index	= scene->mRootNode->mMeshes[0];
	aiMesh* mesh	= scene->mMeshes[mesh_index];
	fillMeshData(mesh);*/

	PostFixDepthSearch( scene, scene->mRootNode ); 
}

View* ModelSpawner::createViewAndSetUpRenderParameters()
{		
	// Create the view
	View* v = nullptr;
	if ( !m_vertexIndices.empty() )
	{
		v = new ElementView;
		static_cast<ElementView*>(v)->setIndexBufferData( 
															&m_vertexIndices[0], 
															static_cast<u32>(m_vertexIndices.size()*sizeof(u32)),
															ElementRenderer::IndexType::U32							// Assimp's mNumVertices' type is u32
		);
	}
	else
		v = new VAOView;

	// Setup the triangle's renderer
	v->setDrawingPrimitive(m_drawingPrimitive);
	v->setNbVerticesToRender(m_nbVerticesToRender);
	//v->setNbVerticesToRender(17298*3);

	return v;
}		 
		 
bool ModelSpawner::createColorData(const Color& _color)
{		 	
	return !m_colors.empty();
}		 
		 
bool ModelSpawner::createNormalData()
{		 
	return !m_normals.empty();
}		 
		 
bool ModelSpawner::create2DTextureMapData()
{
	return !m_2DTexCoords.empty();
}

void ModelSpawner::createVertexData()
{
	
}
