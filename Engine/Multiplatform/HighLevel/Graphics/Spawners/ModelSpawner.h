#pragma once

#include "Graphics/Spawners/ViewSpawner.h"
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

		u32								m_nbVerticesToRender;
		Renderer::DrawingPrimitive		m_drawingPrimitive;
		std::string						m_modelWorkingDir;
		std::vector<std::string>		m_textureBindings;	// Binds a texture name to a binding point. Could be good to create an inner class with resolveTextureBinding
		std::vector<TextureIndex>		m_textureIndices;
		std::vector<u32>				m_vertexIndices;

		u32 resolveTextureBindingPoint(const std::string& _textureName); // May add an element to m_textureBindings
		void fillMeshData(aiMesh* _mesh, const aiMaterial* _material, const aiMatrix4x4& _transformation);
		void ProcessMeshes(const aiScene* _scene, aiNode* _node);
		void PostFixDepthSearch(const aiScene* _scene, aiNode* _root);
		void PreFixDepthSearch(const aiScene* _scene, aiNode* _root);
		void BrutForceSearch(const aiScene* _scene, aiNode* _root);
	};
}