#pragma once

#include "Graphics/Spawners/ViewSpawner.h"

struct aiScene;
struct aiNode;
struct aiMesh;

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
		u32							m_nbVerticesToRender;
		Renderer::DrawingPrimitive	m_drawingPrimitive;
		std::vector<u32>			m_vertexIndices;

		void fillMeshData(aiMesh* _mesh);
		void ProcessMeshes(const aiScene* _scene, aiNode* _node);
		void PostFixDepthSearch(const aiScene* _scene, aiNode* _root);
	};
}