#include "Graphics/Polygons/TriangleFactory.h"
#include "Graphics/VAOView.h"
#include "Graphics/Shader.h"
#include "Debug/NeptuneDebug.h"

using namespace Neptune;

TriangleFactory::TriangleFactory()
{
	// Set vertex data
	float t1[] =
	{
		0.0f,0.0f,0.0f,
		0.5f,0.0f,0.0f,
		0.0f,0.5f,0.0f
	};
	for(unsigned int i = 0; i < sizeof(t1)/sizeof(float); i++)
		m_vertices.push_back(t1[i]);

	// Set color data
	float c1[] =
	{
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f
	};
	for(unsigned int i = 0; i < sizeof(c1)/sizeof(float); i++)
		m_colors.push_back(c1[i]);

	// Create the shader attributes

	GraphicalProgram::ShaderAttribute t1_data =
	{
		0,                                // layout
		GraphicalProgram::Types::FLOAT,  // Type
		3,                              // nb components per value
		sizeof(t1),                    // data size
		&m_vertices[0]                // data
	};

	GraphicalProgram::ShaderAttribute c1_data =
	{
		1,                                // layout
		GraphicalProgram::Types::FLOAT,  // Type
		3,                              // nb components per value
		sizeof(c1),                    // data size
		&m_colors[0]                  // data
	};

	// Add them to the factory
	m_shaderAttributes.push_back(t1_data);
	m_shaderAttributes.push_back(c1_data);

	// Set shader names
	m_vertexShaderName   = "Resources/Shaders/VertexBuffer.vert";
	m_fragmentShaderName = "Resources/Shaders/VertexBuffer.frag";
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