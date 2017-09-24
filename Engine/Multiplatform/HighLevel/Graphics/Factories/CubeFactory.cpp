#include "Graphics/Factories/CubeFactory.h"
#include "Graphics/VAOView.h"

using namespace Neptune;

void CubeFactory::createVertexData()
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

void CubeFactory::createTextureCoordinates()
{
	// Mapping data obtained from http://www.opengl-tutorial.org/fr/beginners-tutorials/tutorial-5-a-textured-cube/
	m_texCoords = {
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
}

View* CubeFactory::createViewAndSetUpRenderParameters()
{
	// Create View
	VAOView* v = new VAOView;

	// Set up the renderer
	v->setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLES);
	v->setNbVerticesToRender(36);

	return v;
}