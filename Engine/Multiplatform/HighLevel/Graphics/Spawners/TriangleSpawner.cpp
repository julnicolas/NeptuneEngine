#include "Graphics/Spawners/TriangleSpawner.h"
#include "Graphics/VAOView.h"

using namespace Neptune;


View* TriangleSpawner::createViewAndSetUpRenderParameters()
{		 
	VAOView* v = new VAOView;

	// Setup the triangle's renderer
	v->setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLES);
	v->setNbVerticesToRender(3);

	return v;
}		 
		 
bool TriangleSpawner::createColorData(const Color& _color)
{		 
	m_colors.push_back(_color);
	m_colors.push_back(_color);
	m_colors.push_back(_color);

	return true;
}		 
		 
bool TriangleSpawner::createNormalData()
{		 
	m_normals.push_back(0.0f); m_normals.push_back(0.0f); m_normals.push_back(-1.0f); // First vertex
	m_normals.push_back(0.0f); m_normals.push_back(0.0f); m_normals.push_back(-1.0f); // Second vertex
	m_normals.push_back(0.0f); m_normals.push_back(0.0f); m_normals.push_back(-1.0f); // Third vertex

	return true;
}		 
		 
bool TriangleSpawner::create2DTextureMapData()
{
	m_2DTexCoords.push_back(0.0f); m_2DTexCoords.push_back(1.0f); // First vertex
	m_2DTexCoords.push_back(1.0f); m_2DTexCoords.push_back(1.0f); // Second vertex
	m_2DTexCoords.push_back(0.0f); m_2DTexCoords.push_back(0.0f); // Third vertex

	return true;
}

void TriangleSpawner::createVertexData()
{
	m_vertices.push_back(0.0f); m_vertices.push_back(0.0f); m_vertices.push_back(0.0f);	// First vertex
	m_vertices.push_back(0.5f); m_vertices.push_back(0.0f); m_vertices.push_back(0.0f);	// Second vertex
	m_vertices.push_back(0.0f); m_vertices.push_back(0.5f); m_vertices.push_back(0.0f); // Third vertex
}