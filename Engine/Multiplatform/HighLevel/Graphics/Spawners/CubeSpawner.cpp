#include "Graphics/Spawners/CubeSpawner.h"
#include "Graphics/VAOView.h"

using namespace Neptune;


const u8 NB_VERTICES_TO_RENDER = 36;

View* CubeSpawner::createViewAndSetUpRenderParameters()
{		 
	VAOView* v = new VAOView;

	// Setup the triangle's renderer
	v->setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLES);
	v->setNbVerticesToRender(NB_VERTICES_TO_RENDER);

	return v;
}		 
		 
bool CubeSpawner::createColorData(const Color& _color)
{		 
	for (u8 i = 0; i < NB_VERTICES_TO_RENDER; i++)
		m_colors.push_back(_color);
	
	return true;
}		 
		 
bool CubeSpawner::createNormalData()
{		 
	m_normals = {
	- 1.0f,  0.0f, 0.0f, // left
    - 1.0f,  0.0f, 0.0f, // left
    - 1.0f,  0.0f, 0.0f, // left
      0.0f,  0.0f,-1.0f, // front
      0.0f,  0.0f,-1.0f, // front
      0.0f,  0.0f,-1.0f, // front
	  0.0f,- 1.0f, 0.0f, // bottom
	  0.0f,- 1.0f, 0.0f, // bottom
	  0.0f,- 1.0f, 0.0f, // bottom
      0.0f,  0.0f,-1.0f, // front
      0.0f,  0.0f,-1.0f, // front
      0.0f,  0.0f,-1.0f, // front
    - 1.0f,  0.0f, 0.0f, // left
    - 1.0f,  0.0f, 0.0f, // left
    - 1.0f,  0.0f, 0.0f, // left
	  0.0f,- 1.0f, 0.0f, // bottom
	  0.0f,- 1.0f, 0.0f, // bottom
	  0.0f,- 1.0f, 0.0f, // bottom
	  0.0f,  0.0f, 1.0f, // back 
	  0.0f,  0.0f, 1.0f, // back
	  0.0f,  0.0f, 1.0f, // back
      1.0f,  0.0f, 0.0f, // right
      1.0f,  0.0f, 0.0f, // right
      1.0f,  0.0f, 0.0f, // right
      1.0f,  0.0f, 0.0f, // right 
      1.0f,  0.0f, 0.0f, // right
      1.0f,  0.0f, 0.0f, // right
      0.0f,  1.0f, 0.0f, // top
      0.0f,  1.0f, 0.0f, // top
      0.0f,  1.0f, 0.0f, // top
      0.0f,  1.0f, 0.0f, // top
      0.0f,  1.0f, 0.0f, // top
      0.0f,  1.0f, 0.0f, // top
      0.0f,  0.0f, 1.0f, // back
      0.0f,  0.0f, 1.0f, // back
      0.0f,  0.0f, 1.0f  // back
	};

	return true;
}		 
		 
bool CubeSpawner::create2DTextureMapData()
{
	// Mapping data obtained from http://www.opengl-tutorial.org/fr/beginners-tutorials/tutorial-5-a-textured-cube/
	m_2DTexCoords = {
	0.000059f, 1.0f-0.000004f,
    0.000103f, 1.0f-0.336048f,
    0.335973f, 1.0f-0.335903f,
    1.000023f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.336024f, 1.0f-0.671877f,
    0.667969f, 1.0f-0.671889f,
    1.000023f, 1.0f-0.000013f,
    0.668104f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.000059f, 1.0f-0.000004f,
    0.335973f, 1.0f-0.335903f,
    0.336098f, 1.0f-0.000071f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.336024f, 1.0f-0.671877f,
    1.000004f, 1.0f-0.671847f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.668104f, 1.0f-0.000013f,
    0.335973f, 1.0f-0.335903f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.668104f, 1.0f-0.000013f,
    0.336098f, 1.0f-0.000071f,
    0.000103f, 1.0f-0.336048f,
    0.000004f, 1.0f-0.671870f,
    0.336024f, 1.0f-0.671877f,
    0.000103f, 1.0f-0.336048f,
    0.336024f, 1.0f-0.671877f,
    0.335973f, 1.0f-0.335903f,
    0.667969f, 1.0f-0.671889f,
    1.000004f, 1.0f-0.671847f,
    0.667979f, 1.0f-0.335851f
	};

	return true;
}

void CubeSpawner::createVertexData()
{
	// Vertex data obtained from http://www.opengl-tutorial.org/fr/beginners-tutorials/tutorial-4-a-colored-cube/
	m_vertices = {
	- 0.5f,- 0.5f,-0.5f, // left
    - 0.5f,- 0.5f, 0.5f, // left
    - 0.5f,  0.5f, 0.5f, // left
      0.5f,  0.5f,-0.5f, // front
    - 0.5f,- 0.5f,-0.5f, // front
    - 0.5f,  0.5f,-0.5f, // front
      0.5f,- 0.5f, 0.5f, // bottom
    - 0.5f,- 0.5f,-0.5f, // bottom
      0.5f,- 0.5f,-0.5f, // bottom
      0.5f,  0.5f,-0.5f, // front
      0.5f,- 0.5f,-0.5f, // front
    - 0.5f,- 0.5f,-0.5f, // front
    - 0.5f,- 0.5f,-0.5f, // left
    - 0.5f,  0.5f, 0.5f, // left
    - 0.5f,  0.5f,-0.5f, // left
      0.5f,- 0.5f, 0.5f, // bottom
    - 0.5f,- 0.5f, 0.5f, // bottom
    - 0.5f,- 0.5f,-0.5f, // bottom
    - 0.5f,  0.5f, 0.5f, // back
    - 0.5f,- 0.5f, 0.5f, // back
      0.5f,- 0.5f, 0.5f, // back
      0.5f,  0.5f, 0.5f, // right
      0.5f,- 0.5f,-0.5f, // right
      0.5f,  0.5f,-0.5f, // right
      0.5f,- 0.5f,-0.5f, // right
      0.5f,  0.5f, 0.5f, // right
      0.5f,- 0.5f, 0.5f, // right
      0.5f,  0.5f, 0.5f, // top
      0.5f,  0.5f,-0.5f, // top
    - 0.5f,  0.5f,-0.5f, // top
      0.5f,  0.5f, 0.5f, // top
    - 0.5f,  0.5f,-0.5f, // top
    - 0.5f,  0.5f, 0.5f, // top
      0.5f,  0.5f, 0.5f, // back
    - 0.5f,  0.5f, 0.5f, // back
      0.5f,- 0.5f, 0.5f	 // back
	};
}
