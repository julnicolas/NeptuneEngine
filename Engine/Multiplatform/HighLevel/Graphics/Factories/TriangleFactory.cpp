#include "Graphics/Factories/TriangleFactory.h"
#include "Graphics/VAOView.h"

using namespace Neptune;

void TriangleFactory::createVertexData()
{
	// Vertex data obtained from http://www.opengl-tutorial.org/fr/beginners-tutorials/tutorial-4-a-colored-cube/
	m_vertices = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.0f
	};
}

void TriangleFactory::createTextureCoordinates()
{
	// Mapping data obtained from http://www.opengl-tutorial.org/fr/beginners-tutorials/tutorial-5-a-textured-cube/
	m_texCoords = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f
	};
}

View* TriangleFactory::createViewAndSetUpRenderParameters()
{
	// Create View
	VAOView* v = new VAOView;

	// Set up the renderer
	v->setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLES);
	v->setNbVerticesToRender(3);

	return v;
}