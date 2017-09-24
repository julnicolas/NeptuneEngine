#include "Graphics/Factories/TriangleFactory.h"
#include "Graphics/VAOView.h"

using namespace Neptune;

void TriangleFactory::createVertexData()
{
	m_vertices = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.0f
	};
}

void TriangleFactory::createTextureCoordinates()
{
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