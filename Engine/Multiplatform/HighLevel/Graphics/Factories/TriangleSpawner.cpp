#include "Graphics/Factories/TriangleSpawner.h"
#include "Graphics/VAOView.h"
#include "Debug/NeptuneDebug.h"

using namespace Neptune;


View* TriangleSpawner::CreateViewAndSetUpRenderParameters()
{		 
	VAOView* v = new VAOView;

	// Setup the triangle's renderer
	Renderer& renderer = v->getRenderer();
	renderer.setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLES);
	renderer.setNbverticesToRender(3);

	return v;
}		 
		 
bool TriangleSpawner::CreateColorData(const Color& _color)
{		 
	m_colors.push_back(_color);
	m_colors.push_back(_color);
	m_colors.push_back(_color);
}		 
		 
bool TriangleSpawner::CreateNormalData()
{		 
	m_normals.push_back(0.0f); m_normals.push_back(0.0f); m_normals.push_back(-1.0f); // First vertex
	m_normals.push_back(0.0f); m_normals.push_back(0.0f); m_normals.push_back(-1.0f); // Second vertex
	m_normals.push_back(0.0f); m_normals.push_back(0.0f); m_normals.push_back(-1.0f); // Third vertex
}		 
		 
bool TriangleSpawner::Create2DTextureMapData()
{
	m_2DTexCoords.push_back(0.0f); m_2DTexCoords.push_back(1.0f); // First vertex
	m_2DTexCoords.push_back(1.0f); m_2DTexCoords.push_back(1.0f); // Second vertex
	m_2DTexCoords.push_back(0.0f); m_2DTexCoords.push_back(0.0f); // Third vertex
}

void TriangleSpawner::createVertexData()
{
	m_vertices.push_back(0.0f); m_vertices.push_back(0.0f); m_vertices.push_back(0.0f);	// First vertex
	m_vertices.push_back(0.5f); m_vertices.push_back(0.0f); m_vertices.push_back(0.0f);	// Second vertex
	m_vertices.push_back(0.0f); m_vertices.push_back(0.5f); m_vertices.push_back(0.0f); // Third vertex
}