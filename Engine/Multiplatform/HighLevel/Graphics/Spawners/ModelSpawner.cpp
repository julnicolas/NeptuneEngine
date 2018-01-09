#include "Graphics/Spawners/ModelSpawner.h"
#include "Graphics/VAOView.h"
#include "Graphics/Texture.h"	// To get textures' standard storage location

#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"
#include <fstream>

#include "Debug/NeptuneDebug.h"
#include <sstream>

#include <stack>

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
	NEP_ASSERT(_mesh != nullptr); // Error, wrong pointer

	u32			num_faces			= _mesh->mNumFaces;
	const u32	num_vertices		= _mesh->mNumVertices;
	const u32	positions_length	= num_vertices*3;
	
	// Populate the vertex buffer
	_vertices.reserve(positions_length);
	for (u32 i = 0, j = 0; i < num_vertices; i++, j +=3)
	{
		aiVector3D& pos = _transformation * _mesh->mVertices[i]; // _transformation == I
		
		_vertices.push_back(pos.x);
		_vertices.push_back(pos.y);
		_vertices.push_back(pos.z);
	}

	// Set number of vertices to render
	_nbVerticesToRender	+= num_faces * 3;		// A face is made up of 3 vertices
}

static void FillVertexIndexData(const aiMesh* _mesh, std::vector<u32>& _vertexIndices)
{
	NEP_ASSERT(_mesh != nullptr); // Error, wrong pointer
	
	const u32	num_faces		= _mesh->mNumFaces;		// Number of primitives present in the mesh (triangles, lines, points)
	const u32	indices_length	= num_faces*3;
	
	_vertexIndices.reserve(indices_length);
	for (u32 face_index = 0; face_index < num_faces; face_index++)
	{
		// The model is broken down into faces; set of vertices which 
		// represent a geometrical surface. A triangle if mesh->mPrimitiveTypes
		// is aiPrimitiveType_TRIANGLE.
		// Each vertex is referenced by an index (position in mesh->mVertices).
		u32  num_indices	= _mesh->mFaces[face_index].mNumIndices;
		u32* indices		= _mesh->mFaces[face_index].mIndices;


		for (u32 i = 0; i < num_indices; i++)
			_vertexIndices.push_back(indices[i]);
	}
}

static void FillColorData(const aiMesh* _mesh, std::vector<Color>& _colors)
{
	NEP_ASSERT(_mesh != nullptr); // Error, wrong pointer
	
	const u32 num_vertices		= _mesh->mNumVertices;
	const u32 colors_length		= num_vertices;
	
	// Populate the color buffer
	if ( *(_mesh->mColors) != nullptr )
	{
		_colors.reserve(colors_length);
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

				_colors.push_back(c);
			}
		}
	}
}

static void FillNormalData(const aiMesh* _mesh, std::vector<float>& _normals)
{
	NEP_ASSERT(_mesh != nullptr); // Error, wrong pointer
	
	const u32 num_vertices		= _mesh->mNumVertices;
	const u32 normals_length	= num_vertices*3;
	
	// Populate the normal buffer
	if ( _mesh->mNormals != nullptr )
	{
		_normals.reserve(normals_length);
		for ( u32 i = 0, j = 0; i < num_vertices; i++, j += 3 )
		{
			_normals.push_back(_mesh->mNormals[i].x);
			_normals.push_back(_mesh->mNormals[i].y);
			_normals.push_back(_mesh->mNormals[i].z);
		}
	}
}

static void FormatTexturePath(std::string& _dst, const char* _path)
{
	NEP_ASSERT(_path != nullptr); // Error, invalid pointer
	_dst = _path;

	// Replace every '\' by '/'
	std::replace_if(_dst.begin(), _dst.end(), [](char _c){return _c == '\\';}, '/' );

	// Remove leading '.'
	if ( _dst[0] == '.' )
		_dst.erase(0, 1);

	// Remove leading '/'
	if ( _dst[0] == '/' )
		_dst.erase(0, 1);
}

void ModelSpawner::FillTextureData(const aiMesh* _mesh, const aiMaterial* _material, u32 _lastIndex)
{
	NEP_ASSERT(_mesh != nullptr);		// Error, wrong pointer
	
	const u32 num_vertices		= _mesh->mNumVertices;
	const u32 texture_length	= num_vertices*2;

	// Populate the texture coordinates buffer
	if ( _mesh->mTextureCoords[0] != nullptr )
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
			
			std::string relative_path_from_model;
			FormatTexturePath(relative_path_from_model, texture_relative_path_from_model.C_Str());
			generateDefaultMapToVerticesAndTextureBindingPoints(_lastIndex, relative_path_from_model.c_str());

			NEP_ASSERT_ERR_MSG( texture_mapping == aiTextureMapping_UV, "Only UV Mapping is supported at the moment. Current mapping mode is %u", texture_mapping );
			NEP_ASSERT_ERR_MSG( get_texture_error == aiReturn_SUCCESS, "Texture couldn't be accessed" );
		}

		// AT the moment only one set of texture coordinate is supported per vertex
		const u8 nb_set_tex_coords = 1; // AI_MAX_NUMBER_OF_TEXTURECOORDS

		m_2DTexCoords.reserve(texture_length);
		for (u8 tex_coord_set = 0; tex_coord_set < nb_set_tex_coords; tex_coord_set++)
		{
			for ( u32 i = 0, j = 0; i < num_vertices; i++, j += 2 )
			{
				m_2DTexCoords.push_back(_mesh->mTextureCoords[tex_coord_set][i].x);
				m_2DTexCoords.push_back(_mesh->mTextureCoords[tex_coord_set][i].y);
			}
		}
	}

	// Check the size of the vertex-to-binding-point map
	// note - multiplied by 2 because the map is of the following format [{last vertex index, binding point},...]
	NEP_ASSERT_ERR_MSG(m_vertexToTextureBindingPointMap.size() <= M_MAX_BINDING_POINT_PER_MODEL * 2, 
		"Too many textures used in model. The engine only supports %u binding points per model.", M_MAX_BINDING_POINT_PER_MODEL);
}

static std::string Debug_MapDrawingPrimitiveToString(u32 _prim)
{
	switch (_prim)
	{
	case aiPrimitiveType_POINT:
		return "aiPrimitiveType_POINT";

	case aiPrimitiveType_LINE:
		return "aiPrimitiveType_LINE";

	case aiPrimitiveType_TRIANGLE:
		return "aiPrimitiveType_TRIANGLE";

	case aiPrimitiveType_POLYGON:
		return "aiPrimitiveType_POLYGON";

	default:
		return "UNKOWN PRIMITIVE";
	}
}

void ModelSpawner::fillMeshData(aiMesh* _mesh, const aiMaterial* _material, const aiMatrix4x4& _transformation)
{
	NEP_ASSERT(_mesh != nullptr); // Error, wrong pointer
	
	// check rendering primitive
	// just triangles are supported at the moment
	NEP_ASSERT_ERR_MSG( !(_mesh->mPrimitiveTypes & aiPrimitiveType_POINT) &&
						!(_mesh->mPrimitiveTypes & aiPrimitiveType_LINE)  &&
						!(_mesh->mPrimitiveTypes & aiPrimitiveType_POLYGON), 
						"Wrong primitive, only aiPrimitiveType_TRIANGLE is supported (current is %s).", 
						Debug_MapDrawingPrimitiveToString(_mesh->mPrimitiveTypes).c_str());
	
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
	NEP_ASSERT_ERR_MSG(!fin.fail(), "File not found. Path : %s", _modelPath); // Error file doesn't exist

	// Load mesh
	const aiScene* scene = nullptr; 
    scene = importer.ReadFile( _modelPath, aiProcess_ConvertToLeftHanded);
	
	NEP_ASSERT(scene != nullptr); // Error Import failed
	
	NEP_ASSERT_ERR_MSG(!(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE), "Error in file's content... something is wrong with the data.");

	// Check content
	NEP_ASSERT(scene->mNumMeshes > 0); // Error the file doesn't define any mesh

	// P O P U L A T E   T H E   B U F F E R S
    
	u32 num_meshses	= scene->mNumMeshes;
	aiNode* node	= scene->mRootNode;
	PostFixDepthSearch( scene, scene->mRootNode );
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
static u32 ResolveTextureBindingPoint(std::vector<std::string>& _textureNameList, const std::string& _textureName)
{
	// Does the texture have a binding point allocated?
	for (u32 i = 0; i < _textureNameList.size(); i++)
	{
		if (_textureNameList[i] == _textureName)
			return i;	// Yes, return it
	}

	// Texture doesn't have a binding point yet
	_textureNameList.push_back(_textureName);				// Allocating a binding point
	return static_cast<u32>(_textureNameList.size())-1;	// Retrieve the binding point
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
		// to_string must be used to avoid (u8) 0 to be considered as '\0'
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

void ModelSpawner::generateDefaultMapToVerticesAndTextureBindingPoints(u32 _meshLastIndex, const char* _textureRelativePathFromModel)
{
	NEP_ASSERT(_textureRelativePathFromModel != nullptr); // Error, Invalid path

	std::string texture_relative_path = Texture::GetStandardDir() + std::string(_textureRelativePathFromModel);

	// Map texture name to binding point
	m_textureBindingPoints[texture_relative_path] = ResolveTextureBindingPoint(m_textureNames, texture_relative_path.c_str());

	// Map last vertex for sampling current texture to texture name
	TextureBindingTableEntry binding_map;
	binding_map.m_textureName		= texture_relative_path;
	binding_map.m_lastVertexIndex	= _meshLastIndex;
	m_vertexToTextureBindingPointMap.push_back(binding_map);
}

// Vector of {lastVertex, bindingPoint}
void ModelSpawner::mapVerticesToTextureBindingPoints(std::vector<u32>& _outTable) const
{
	_outTable.clear();

	u32 i = 0;
	for ( const auto& entry : m_vertexToTextureBindingPointMap )
	{
		_outTable.push_back(entry.m_lastVertexIndex);

		NEP_ASSERT( m_textureBindingPoints.find(entry.m_textureName) != m_textureBindingPoints.end() ); // Error, incoherent texture names
		u8 binding = m_textureBindingPoints.find(entry.m_textureName)->second;
		_outTable.push_back(binding); // Get texture's binding point

		i++;
	}
}