#include "Graphics/Factories/PlanFactory.h"
#include "Graphics/VAOView.h"

using namespace Neptune;

void PlanFactory::createVertexData()
{
	m_vertices = {
		0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};
}

void PlanFactory::createTextureCoordinates()
{
	m_texCoords = {
		0.0f,1.0f,
		1.0f,1.0f,
		0.0f,0.0f,
		1.0f,0.0f
	};
}

View* PlanFactory::createViewAndSetUpRenderParameters()
{
	// Create View
	VAOView* v = new VAOView;

	// Set up the renderer
	v->setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLE_STRIP);
	v->setNbVerticesToRender(4);

	return v;
}