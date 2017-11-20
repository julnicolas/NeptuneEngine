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

		//void getTextureBindingInfo(std::unordered_map<const char*, u8>& _info)			const;
		void getTextureBindingInfo(std::unordered_map<std::string, u8>& _info)			const;
		//void setTextureBindingInfo(const std::unordered_map<const char*, u8>& _info);
		void setTextureBindingInfo(const std::unordered_map<std::string, u8>& _info);
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
		struct BindingInfo
		{
			std::string	m_textureName;
			//const char* m_textureName	= nullptr;
			u8			m_binding		= 0;
		};

		/// \brief	Contains information to choose which texture to sample from
		///			for each and every vertex.
		///	\Note	This struct is supposed to be contained in array.
		struct TextureIndex
		{
			/// \brief		Value used to choose which texture to sample from for the current vertex.
			///				Read the following example for further information.
			/// \example	Let us consider an array of TextureIndex called tindex.
			///				If tindex[i-1].m_lastTextureIndex < vertex_id <= tindex[i].m_lastTextureIndex
			///				then use texture whose texture binding is tindex[i].m_textureBinding.
			u32	m_lastTextureIndex;
			u8	m_textureBinding;		/// Texture binding number to use for the current vertex
		};

		struct BindingMap
		{
			u32			m_lastVertexIndex;
			std::string m_textureName;
		};

		std::string							m_modelWorkingDir;
		std::unordered_map<std::string, u8>	m_textureBindingInfo;	/// key : texture's name, value : binding point
		std::vector<BindingMap>				m_textureBindingTable;	/// Table to map a texture to a vertex
		std::vector<std::string>			m_textureNames;

		u32								m_nbVerticesToRender;
		Renderer::DrawingPrimitive		m_drawingPrimitive;
		std::vector<u32>				m_vertexIndices;

		u32 resolveTextureBindingPoint(const std::string& _textureName); // May add an element to m_textureBindings
		void fillMeshData(aiMesh* _mesh, const aiMaterial* _material, const aiMatrix4x4& _transformation);
		void ProcessMeshes(const aiScene* _scene, aiNode* _node);
		void PostFixDepthSearch(const aiScene* _scene, aiNode* _root);
		void PreFixDepthSearch(const aiScene* _scene, aiNode* _root);
		void BrutForceSearch(const aiScene* _scene, aiNode* _root);
	};
}