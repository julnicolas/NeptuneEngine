#include "Graphics/Factories/PlanFactory.h"
#include "Graphics/VAOView.h"

#include "Graphics/GraphicalEnumMappingFunctions.h"

using namespace Neptune;

void PlanFactory::createVertexData()
{
	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.5f);
	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.5f);
	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.5f);
	m_vertices.push_back(0.5f);
	m_vertices.push_back(0.0f);
}

void PlanFactory::createTextureCoordinates()
{
	m_texCoords.push_back(0.0f); m_texCoords.push_back(1.0f);
	m_texCoords.push_back(1.0f); m_texCoords.push_back(1.0f);
	m_texCoords.push_back(0.0f); m_texCoords.push_back(0.0f);
	m_texCoords.push_back(1.0f); m_texCoords.push_back(0.0f);
}

View* PlanFactory::createViewAndSetUpRenderParameters()
{
	// Create View
	VAOView* v = new VAOView;

	// Set up the triangle's renderer
	v->setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLE_STRIP);
	v->setNbVerticesToRender(4);

	return v;
}