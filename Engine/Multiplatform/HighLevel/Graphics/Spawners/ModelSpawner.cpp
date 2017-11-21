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
#include <sstream>

#include <stack>

#include <algorithm>

using namespace Neptune;

static const aiMaterial* GetMaterial(const aiScene* _scene, const aiMesh* _mesh)
{
	u32 material_index = _mesh->mMaterialIndex;
	if ( material_index < _scene->mNumMaterials )
		return _scene->mMaterials[material_index];

	return nullptr;
}

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

void ModelSpawner::BrutForceSearch(const aiScene* _scene, aiNode* _root)
{
	aiMatrix4x4 I;

	u32 num_meshes = _scene->mNumMeshes;
	for (u32 i = 0; i < num_meshes; i++)
	{
		aiMesh* mesh = _scene->mMeshes[i]; 
		fillMeshData( mesh, GetMaterial(_scene, mesh), I );
	}
}

void ModelSpawner::PreFixDepthSearch(const aiScene* _scene, aiNode* _root)
{
	NEP_ASSERT( _root  != nullptr ); // Error invalid pointer
	NEP_ASSERT( _scene != nullptr ); // Error invalid pointer
	
	std::stack<Node, std::vector<Node>> stack;
	Node node;
	node.m_node			= _root;
	node.m_childIndex	= 0;

	unsigned int depth = 0;
	do 
	{
		if ( node.m_childIndex == 0 )
				ProcessMeshes( _scene, node.m_node );

		u32 num_children = node.m_node->mNumChildren;
		if ( num_children != 0 && node.m_childIndex < num_children )
		{
			node.m_childIndex++;
			stack.push(node);

			node.m_node = node.m_node->mChildren[node.m_childIndex-1];
			node.m_childIndex = 0;
			depth++;
		}
		else
		{
			if ( !stack.empty() ) // Get parent
			{
				node = stack.top();
				stack.pop();
				depth--;
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
			aiMesh* mesh = _scene->mMeshes[mesh_index]; 
			fillMeshData( mesh, GetMaterial(_scene, mesh), _node->mTransformation );
	}
}

static void FillVertexData(const aiMesh* _mesh, const aiMatrix4x4& _transformation, std::vector<float>& _vertices, u32& _nbVerticesToRender)
{
	u32			num_faces			= _mesh->mNumFaces;
	const u32	num_vertices		= _mesh->mNumVertices;
	const u32	positions_length	= num_vertices*3;
	float*		positions			= new float[positions_length];
	
	// Populate the vertex buffer
	for (u32 i = 0, j = 0; i < num_vertices; i++, j +=3)
	{
		aiVector3D& pos = _transformation * _mesh->mVertices[i]; // _transformation == I
		
		positions[j]   = pos.x;
		positions[j+1] = pos.y;
		positions[j+2] = pos.z;
	}
	_vertices.insert(_vertices.cend(), positions, positions + positions_length);

	// Set number of vertices to render
	_nbVerticesToRender	+= num_faces * 3;		// A face is made up of 3 vertices

	// Clean up
	delete[] positions;
}

static void FillVertexIndexData(const aiMesh* _mesh, std::vector<u32>& _vertexIndices)
{
	const u32	num_faces		= _mesh->mNumFaces;		// Number of primitives present in the mesh (triangles, lines, points)
	const u32	indices_length	= num_faces*3;
	u32*		indices_array	= new u32[indices_length];
	
	u32 face_index_in_array = 0;
	for (u32 face_index = 0; face_index < num_faces; face_index++)
	{
		// The model is broken down into faces; set of vertices which 
		// represent a geometrical surface. A triangle if mesh->mPrimitiveTypes
		// is aiPrimitiveType_TRIANGLE.
		// Each vertex is referenced by an index (position in mesh->mVertices).
		u32  num_indices	= _mesh->mFaces[face_index].mNumIndices;
		u32* indices		= _mesh->mFaces[face_index].mIndices;


		for (u32 i = 0; i < num_indices; i++)
			indices_array[face_index_in_array+i] = indices[i];

		face_index_in_array += num_indices;
	}
	_vertexIndices.insert(_vertexIndices.cend(), indices_array, indices_array+indices_length);

	delete[] indices_array;
}

static void FillColorData(const aiMesh* _mesh, std::vector<Color>& _colors)
{
	const u32 num_vertices		= _mesh->mNumVertices;
	const u32 colors_length		= num_vertices;
	Color*	colors				= new Color[colors_length];
	
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

				colors[i] = c;
			}
		}
	}
	_colors.insert(_colors.cend(), colors, colors + colors_length);

	delete[] colors;
}

static void FillNormalData(const aiMesh* _mesh, std::vector<float>& _normals)
{
	const u32 num_vertices		= _mesh->mNumVertices;
	const u32 normals_length	= num_vertices*3;
	float*  normals				= new float[normals_length];
	
	// Populate the normal buffer
	if ( _mesh->mNormals != nullptr )
	{
		for ( u32 i = 0, j = 0; i < num_vertices; i++, j += 3 )
		{
			normals[j]   = _mesh->mNormals[i].x;
			normals[j+1] = _mesh->mNormals[i].y;
			normals[j+2] = _mesh->mNormals[i].z;
		}
	}
	_normals.insert(_normals.cend(), normals, normals + normals_length);

	delete[] normals;
}

void ModelSpawner::FillTextureData(const aiMesh* _mesh, const aiMaterial* _material, u32 _lastIndex)
{
	const u32 num_vertices		= _mesh->mNumVertices;
	const u32 texture_lenght	= num_vertices*2;
	float*	tex_coords			= new float[texture_lenght];

	// Populate the texture coordinates buffer
	if ( _mesh->mTextureCoords != nullptr )
	{
		// Get Texture's name or id for further processing
		if (_material != nullptr)
		{
			u32 texture_count		= _material->GetTextureCount(aiTextureType_DIFFUSE);
			NEP_ASSERT_ERR_MSG( texture_count == 1, "Only one diffuse color texture is supported per mesh. Current mesh has %u.", texture_count );

			aiString			texture_relative_path_from_model; // Init to empty string
			aiTextureMapping	texture_mapping = aiTextureMapping_OTHER;
			u32					uv_index		= 0;
			auto get_texture_error	= _material->GetTexture(	aiTextureType_DIFFUSE,
																0,
																&texture_relative_path_from_model,
																&texture_mapping,
																&uv_index
															);
			
			generateDefaultTextureBinding(_lastIndex, texture_relative_path_from_model.C_Str());

			NEP_ASSERT_ERR_MSG( texture_mapping == aiTextureMapping_UV, "Only UV Mapping is supported at the moment. Current mapping mode is %u", texture_mapping );
			NEP_ASSERT_ERR_MSG( get_texture_error == aiReturn_SUCCESS, "Texture couldn't be accessed" );
		}

		// AT the moment only one set of texture coordinate is supported per vertex
		const u8 nb_set_tex_coords = 1; // AI_MAX_NUMBER_OF_TEXTURECOORDS

		for (u8 tex_coord_set = 0; tex_coord_set < nb_set_tex_coords; tex_coord_set++)
		{
			for ( u32 i = 0, j = 0; i < num_vertices; i++, j += 2 )
			{
				tex_coords[j]   = _mesh->mTextureCoords[tex_coord_set][i].x;
				tex_coords[j+1] = _mesh->mTextureCoords[tex_coord_set][i].y;
				//tex_coords[j+2] = _mesh->mNormals[i].z;
			}
		}
	}
	m_2DTexCoords.insert(m_2DTexCoords.cend(), tex_coords, tex_coords + texture_lenght);

	delete[] tex_coords;
}

void ModelSpawner::fillMeshData(aiMesh* _mesh, const aiMaterial* _material, const aiMatrix4x4& _transformation)
{
	// check rendering primitive
	// just triangles are supported at the moment
	NEP_ASSERT_ERR_MSG( !(_mesh->mPrimitiveTypes & aiPrimitiveType_POINT) &&
						!(_mesh->mPrimitiveTypes & aiPrimitiveType_LINE)  &&
						!(_mesh->mPrimitiveTypes & aiPrimitiveType_POLYGON), "Only triangles are supported.");
	
	NEP_ASSERT_ERR_MSG((_mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) != 0, "Error, no drawing primitive has been provided. Check your model file.");

	// Texture indexing
	// Get current mesh's first vertex's index and
	// Get current mesh's last vertex's index
	u32 mesh_first_index = static_cast<u32>(m_vertices.size())/3 - 1;			// divided by 3 because we want the number of sets of x,y,z coordinates.
	u32 mesh_last_index  = mesh_first_index + _mesh->mNumVertices;

	FillVertexData(_mesh, _transformation, m_vertices, m_nbVerticesToRender);
	FillVertexIndexData(_mesh, m_vertexIndices);
	FillColorData(_mesh, m_colors);
	FillNormalData(_mesh,m_normals);
	FillTextureData(_mesh, _material, mesh_last_index);
}

ModelSpawner::ModelSpawner(GraphicsProgram* _pgm, const char* _modelPath):
	ViewSpawner(_pgm),
	m_nbVerticesToRender(0), 
	m_drawingPrimitive(Renderer::DrawingPrimitive::TRIANGLES)
{
	// L O A D   M O D E L
	NEP_ASSERT(_modelPath != nullptr); // Error: Invalid file name
	NEP_ASSERT_ERR_MSG(std::string(_modelPath).find('\\') == std::string::npos,
		"Error, paths should only contain slashes '/'.\n Path provided is %s\n", _modelPath); // Check no forward slashes is present

	// Create an instance of the Importer class
    Assimp::Importer importer;

    //check if file exists
    std::ifstream fin(_modelPath);
	NEP_ASSERT(!fin.fail()); // Error file doesn't exist

	// Load mesh
	const aiScene* scene = nullptr; 
    scene = importer.ReadFile( _modelPath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	
	NEP_ASSERT(scene != nullptr); // Error Import failed
	
	NEP_ASSERT_ERR_MSG(!(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE), "Error in file content... something is wrong with the data.");

	// Check content
	NEP_ASSERT(scene->mNumMeshes > 0); // Error the file doesn't define any mesh

	// Set working directory
	m_modelWorkingDir = _modelPath;
	m_modelWorkingDir = m_modelWorkingDir.substr(0,m_modelWorkingDir.find_last_of('/')+1);

	// P O P U L A T E   T H E   B U F F E R S
    
	u32 num_meshses	= scene->mNumMeshes;
	aiNode* node = scene->mRootNode;

	//BrutForceSearch( scene, scene->mRootNode );
	PostFixDepthSearch( scene, scene->mRootNode );
	//PreFixDepthSearch( scene, scene->mRootNode );
}

View* ModelSpawner::createViewAndSetUpRenderParameters()
{		
	// Create the view
	View* v = new VAOView;

	// Setup the triangle's renderer
	v->setDrawingPrimitive(m_drawingPrimitive);
	v->setNbVerticesToRender(m_nbVerticesToRender);

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

// Binding point retrieved is the position of _textureName in m_textureNames
u32 ModelSpawner::resolveTextureBindingPoint(const std::string& _textureName)
{
	// Does the texture have a binding point allocated?
	for (u32 i = 0; i < m_textureNames.size(); i++)
	{
		if (m_textureNames[i] == _textureName)
			return i;	// Yes, return it
	}

	// Texture doesn't have a binding point yet
	m_textureNames.push_back(_textureName);				// Allocating a binding point
	return static_cast<u32>(m_textureNames.size())-1;	// Retrieve the binding point
}

// Returns true if every key matches
static bool DEBUG_CheckKeys(const std::unordered_map<std::string, u8>& _map1, const std::unordered_map<std::string, u8>& _map2)
{
	// Check that every key is found only once. Since it is sure both containers have the same size (determined in caller function)
	// then it is possible to conclude that both containers have exactly the same keys.
	// If container sizes are different, it means that _map2 has the exact same keys as _map1, plus potentially others.
	for (const auto& entry : _map1)
	{
		if ( _map2.count(entry.first) != 1 )
			return false;
	}

	return true;
}

static std::string DEBUG_ToString(const std::unordered_map<std::string, u8>& _map1)
{
	std::stringstream stream;
	stream << "[";

	for (const auto& entry : _map1)
	{
		stream << "{" << entry.first << " : " << std::to_string(entry.second) << "}, ";
	}
	stream << "]";

	std::string str = stream.str();
	return stream.str();
}

void ModelSpawner::getTextureBindingPoints(std::unordered_map<std::string, u8>& _bindingPoints) const
{
	_bindingPoints = m_textureBindingPoints;
}

void ModelSpawner::setTextureBindingPoints(const std::unordered_map<std::string, u8>& _bindingPoints)
{
	NEP_ASSERT_ERR_MSG(_bindingPoints.size() == m_textureBindingPoints.size(), "Error, binding info doesn't match.");
	NEP_ASSERT_ERR_MSG( DEBUG_CheckKeys(_bindingPoints, m_textureBindingPoints), "Error, texture names are different.\nYou provided : %s\nWas expected : %s", 
		DEBUG_ToString(_bindingPoints).c_str(), DEBUG_ToString(m_textureBindingPoints).c_str());

	m_textureBindingPoints = _bindingPoints;
}

void ModelSpawner::generateDefaultTextureBinding(u32 _meshLastIndex, const char* _textureRelativePathFromModel)
{
	NEP_ASSERT(_textureRelativePathFromModel != nullptr); // Error, Invalid path

	std::string texture_relative_path = m_modelWorkingDir + std::string(_textureRelativePathFromModel);

	// Map texture name to binding point
	m_textureBindingPoints[texture_relative_path] = resolveTextureBindingPoint(texture_relative_path.c_str());

	// Map last vertex for sampling current texture to texture name
	TextureBindingTableEntry binding_map;
	binding_map.m_textureName		= texture_relative_path;
	binding_map.m_lastVertexIndex	= _meshLastIndex;
	m_textureBindingTable.push_back(binding_map);
}

// Vector of {lastVertex, bindingPoint}
void ModelSpawner::generateTextureBindingTable(std::vector<u32>& _table)
{
	_table.clear();

	u32 i = 0;
	for ( const auto& entry : m_textureBindingTable )
	{
		_table.push_back(entry.m_lastVertexIndex);

		NEP_ASSERT( m_textureBindingPoints.find(entry.m_textureName) != m_textureBindingPoints.end() ); // Error, incoherent texture names
		u8 binding = m_textureBindingPoints[entry.m_textureName];
		_table.push_back(binding); // Get texture's binding point

		i++;
	}
}