#include "Graphics/Polygons/TriangleFactory.h"
#include "Graphics/VAOView.h"
#include "Graphics/Shader.h"
#include "Debug/NeptuneDebug.h"

using namespace Neptune;

TriangleFactory::TriangleFactory()
{
	initData(0.5f, 0.5f, 0.5f);
}

TriangleFactory::TriangleFactory(float r, float g, float b)
{
	initData(r, g, b);
}

VAOView* TriangleFactory::create()
{
	VAOView* v = new VAOView;
	
	Renderer& renderer = v->getRenderer();

	// Create the triangle's renderer
	renderer.setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLES);
	renderer.setNbverticesToRender(3);

	// Create the shaders to display the triangle
	Shader vert(m_vertexShaderName.c_str(),GL_VERTEX_SHADER);
	Shader frag(m_fragmentShaderName.c_str(),GL_FRAGMENT_SHADER);

	// Create the program to display a triangle
	{
		GraphicalProgram& pgm = renderer.createProgram();
		pgm.add(vert.getId());
		pgm.add(frag.getId());
		pgm.addShaderAttribute(m_shaderAttributes[0]);
		pgm.addShaderAttribute(m_shaderAttributes[1]);
		pgm.build();
	}

	return v;
}

void TriangleFactory::initData(float r, float g, float b)
{
	// Set vertex data
	m_vertices.push_back( 0.0f );
	m_vertices.push_back( 0.0f );
	m_vertices.push_back( 0.0f );
	m_vertices.push_back( 0.5f );
	m_vertices.push_back( 0.0f );
	m_vertices.push_back( 0.0f );
	m_vertices.push_back( 0.0f );
	m_vertices.push_back( 0.5f );
	m_vertices.push_back( 0.0f );

	// Set color data
	m_colors.push_back(r); m_colors.push_back(g); m_colors.push_back(b);
	m_colors.push_back(r); m_colors.push_back(g); m_colors.push_back(b);
	m_colors.push_back(r); m_colors.push_back(g); m_colors.push_back(b);

	// Create the shader attributes

	GraphicalProgram::ShaderAttribute t1_data =
	{
		0,                                          // layout
		GraphicalProgram::Types::FLOAT,            // Type
		3,                                        // nb components per value
		m_vertices.size()*sizeof(m_vertices[0]), // data size
		&m_vertices[0]                          // data
	};

	GraphicalProgram::ShaderAttribute c1_data =
	{
		1,                                      // layout
		GraphicalProgram::Types::FLOAT,        // Type
		3,                                    // nb components per value
		m_colors.size()*sizeof(m_colors[0]), // data size
		&m_colors[0]                        // data
	};

	// Add them to the factory
	m_shaderAttributes.push_back(t1_data);
	m_shaderAttributes.push_back(c1_data);

	// Set shader names
	m_vertexShaderName   = "Resources/Shaders/VertexBuffer.vert";
	m_fragmentShaderName = "Resources/Shaders/VertexBuffer.frag";
}