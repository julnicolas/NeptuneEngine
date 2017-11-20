#pragma once

#include "Graphics/Spawners/ViewSpawner.h"
#include <unordered_map>
#include <string> // WIP


struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

template <typename T>
class  aiMatrix4x4t;
typedef aiMatrix4x4t<float> aiMatrix4x4;

namespace Neptune
{
	/// \class Instantiate models. The class is limited t the instantiation of 1 mesh per file.
	class ModelSpawner : public ViewSpawner
	{
	public:
		//
		// C O N S T R U C T O R S
		//
		
		ModelSpawner(GraphicsProgram* _pgm, const char* _modelPath);

		//
		// D E F A U L T   G E N E R A T E D   M E T H O D S
		//

		virtual ~ModelSpawner()									= default;

		ModelSpawner(const ModelSpawner&)						= delete;
		ModelSpawner(ModelSpawner&&)							= delete;
		virtual ModelSpawner& operator=(const ModelSpawner&)	= delete;
		virtual ModelSpawner& operator=(ModelSpawner&&)			= delete;

		//
		// M O D E L ' S   T E X T U R E   M A N A G E M E N T   M E T H O D S
		//

		/// Retrieves an unordered_map of {texture path, binding point}
		void getTextureBindingPoints(std::unordered_map<std::string, u8>& _bindingPoints)			const;
		//void getTextureBindingInfo(std::unordered_map<const char*, u8>& _info)			const;

		/// Sets new binding points for textures. One should call getTextureBindingPoints() first to get the base information.
		/// \example	std::unordered_map<std::string, u8> binding_points;
		///				getTextureBindingPoints(binding_points);	// Returns [{"Resources/a.png" : 0}, {"b.png" : 1}]
		///				binding_points["b.png"] = 3;				// Useful if b.png is already used for rendering and bound to index 3
		///				setTextureBindingPoints(binding_points);
		void setTextureBindingPoints(const std::unordered_map<std::string, u8>& _bindingPoints);
		//void setTextureBindingInfo(const std::unordered_map<const char*, u8>& _info);

		/// \brief		Generates a table with the following format - {last_vertex, binding_point}.
		///				Where last_vertex is the index of the last vertex to be used with the texture 
		///				at binding point binding_point.
		/// \note		This table is used by shaders implementing diffuse multi-texturing.
		///
		/// \example	Let us consider a table called bindings.
		///				If bindings[0] < vertex_id <= bindings[2]
		///				then use texture whose texture binding is bindings[3].
		void generateTextureBindingTable(std::vector<u32>& _table);

		//
		// P U R E   V I R T U A L   M E T H O D S 
		//

		void  createVertexData()				override;													/// Creates the vertex position data
		bool  createColorData(const Color& _c)	override;													/// Creates per-vertex color data, if the data were already created, the color will be changed (for all the view instantiated by the factory). The input color for this method is not referenced.
		bool  createNormalData()				override;													/// Creates the normals at every vertex of the view
		bool  create2DTextureMapData()			override;

	protected:
		/// \brief In this method you must dynamically allocate a View object
		/// (called v). Then call v->getRenderer().setDrawingPrimitive(_prim)  
		/// and v->getRenderer().setNbverticesToRender(nb).
		View* createViewAndSetUpRenderParameters() override;

	private:
		using TextureName	= std::string;
		using BindingPoint	= u8;

		/// Entry in the texture binding table
		/// A texture binding table is a table that maps vertices to a texture
		/// for sampling from shaders.
		struct TextureBindingTableEntry
		{
			/// \brief		Value used to choose which texture to sample from for the current vertex.
			///				Read the following example for further information.
			/// \example	Let us consider an array of TextureBindingTableEntry called tindex.
			///				If tindex[i-1].m_lastTextureIndex < vertex_id <= tindex[i].m_lastTextureIndex
			///				then use texture whose texture binding is m_textureBindingPoints[tindex[i].m_textureName].
			u32				m_lastVertexIndex;
			std::string		m_textureName;		/// Texture name. Used as a key to get texture's binding point from m_textureBindingPoints.
		};

		std::string										m_modelWorkingDir;
		std::unordered_map<TextureName, BindingPoint>	m_textureBindingPoints;	/// key : texture's name, value : binding point
		std::vector<TextureBindingTableEntry>			m_textureBindingTable;	/// Table to map a texture to a vertex
		std::vector<std::string>						m_textureNames;

		u32												m_nbVerticesToRender;
		Renderer::DrawingPrimitive						m_drawingPrimitive;
		std::vector<u32>								m_vertexIndices;

		u32 resolveTextureBindingPoint(const std::string& _textureName); // May add an element to m_textureBindings
		void generateDefaultTextureBinding(u32 _meshLastIndex, const char* _textureRelativePathFromModel);
		
		
		void fillMeshData(aiMesh* _mesh, const aiMaterial* _material, const aiMatrix4x4& _transformation);
		void ProcessMeshes(const aiScene* _scene, aiNode* _node);
		void PostFixDepthSearch(const aiScene* _scene, aiNode* _root);
		void PreFixDepthSearch(const aiScene* _scene, aiNode* _root);
		void BrutForceSearch(const aiScene* _scene, aiNode* _root);
	};
}