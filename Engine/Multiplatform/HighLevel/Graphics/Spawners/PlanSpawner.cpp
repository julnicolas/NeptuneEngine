#include "Graphics/Spawners/PlanSpawner.h"
#include "Graphics/VAOView.h"
#include "Debug/NeptuneDebug.h"

using namespace Neptune;


View* PlanSpawner::createViewAndSetUpRenderParameters()
{		 
	VAOView* v = new VAOView;

	// Setup the triangle's renderer
	v->setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLE_STRIP);
	v->setNbVerticesToRender(4);

	return v;
}		 
		 
bool PlanSpawner::createColorData(const Color& _color)
{		 
	m_colors.push_back(_color);
	m_colors.push_back(_color);
	m_colors.push_back(_color);
	m_colors.push_back(_color);

	return true;
}		 
		 
bool PlanSpawner::createNormalData()
{		 
	m_normals.push_back(0.0f); m_normals.push_back(0.0f); m_normals.push_back(-1.0f); // First  vertex
	m_normals.push_back(0.0f); m_normals.push_back(0.0f); m_normals.push_back(-1.0f); // Second vertex
	m_normals.push_back(0.0f); m_normals.push_back(0.0f); m_normals.push_back(-1.0f); // Third  vertex
	m_normals.push_back(0.0f); m_normals.push_back(0.0f); m_normals.push_back(-1.0f); // Fourth vertex

	return true;
}		 
		 
bool PlanSpawner::create2DTextureMapData()
{
	m_2DTexCoords.push_back(0.0f); m_2DTexCoords.push_back(1.0f); // First  vertex
	m_2DTexCoords.push_back(1.0f); m_2DTexCoords.push_back(1.0f); // Second vertex
	m_2DTexCoords.push_back(0.0f); m_2DTexCoords.push_back(0.0f); // Third  vertex
	m_2DTexCoords.push_back(1.0f); m_2DTexCoords.push_back(0.0f); // Fourth vertex

	return true;
}

void PlanSpawner::createVertexData()
{
	m_vertices.push_back(0.0f); m_vertices.push_back(0.0f); m_vertices.push_back(0.0f); // First  vertex
	m_vertices.push_back(0.5f); m_vertices.push_back(0.0f); m_vertices.push_back(0.0f);	// Second vertex
	m_vertices.push_back(0.0f); m_vertices.push_back(0.5f); m_vertices.push_back(0.0f);	// Third  vertex
	m_vertices.push_back(0.5f); m_vertices.push_back(0.5f); m_vertices.push_back(0.0f);	// Fourth vertex
}