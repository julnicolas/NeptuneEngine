#include "Graphics/Factories/CubeFactory.h"
#include "Graphics/VAOView.h"
#include "Graphics/Shader.h"
#include "Debug/NeptuneDebug.h"

using namespace Neptune;

CubeFactory::CubeFactory()
{
	initCubeData(0.5f, 0.5f, 0.5f);
}

CubeFactory::CubeFactory(float r, float g, float b)
{
	initCubeData(r, g, b);
}

VAOView* CubeFactory::create()
{
	VAOView* v = new VAOView;
	
	// Add the MV matrix

	GraphicsProgram::UniformVarInput mv("ModelView",
		GraphicsProgram::FLOAT,
		4,
		4,
		16*sizeof(float),
		v->getTransform().getDataPtr() );

	// Create the renderer

	Renderer& renderer = v->getRenderer();

	renderer.setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLES);
	renderer.setNbverticesToRender(NB_VERTICES_TO_RENDER);

	// Create the shaders to display the triangle
	Shader vert(m_vertexShaderName.c_str(),GL_VERTEX_SHADER);
	Shader frag(m_fragmentShaderName.c_str(),GL_FRAGMENT_SHADER);

	// Create the program to display a triangle
	{
		GraphicsProgram& pgm = renderer.createProgram();
		pgm.add(vert.getId());
		pgm.add(frag.getId());
		pgm.addShaderAttribute(m_shaderAttributes[0]);
		pgm.addShaderAttribute(m_shaderAttributes[1]);
		pgm.addUniformVariable( mv );
		pgm.build();
	}

	return v;
}

void CubeFactory::initCubeData(float r, float g, float b)
{
	// Set vertex data
	float t1[] =
	{
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		-0.5f,0.5f,0.5f,
		0.5f,0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,0.5f,-0.5f,
		0.5f,-0.5f,0.5f,
		-0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,0.5f,0.5f,
		-0.5f,0.5f,-0.5f,
		0.5f,-0.5f,0.5f,
		-0.5f,-0.5f,0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,0.5f,0.5f,
		-0.5f,-0.5f,0.5f,
		0.5f,-0.5f,0.5f,
		0.5f,0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,0.5f,0.5f,
		0.5f,-0.5f,0.5f,
		0.5f,0.5f,0.5f,
		0.5f,0.5f,-0.5f,
		-0.5f,0.5f,-0.5f,
		0.5f,0.5f,0.5f,
		-0.5f,0.5f,-0.5f,
		-0.5f,0.5f,0.5f,
		0.5f,0.5f,0.5f,
		-0.5f,0.5f,0.5f,
		0.5f,-0.5f,0.5f
	};
	for(unsigned int i = 0; i < sizeof(t1)/sizeof(float); i++)
		m_vertices.push_back(t1[i]);

	// Set color data

	float color[3]; color[0] = r; color[1] = g; color[2] = b;

	for(unsigned int i = 0; i < NB_VERTICES_TO_RENDER; i++)
	{
		m_colors.push_back(color[0]);
		m_colors.push_back(color[1]);
		m_colors.push_back(color[2]);
	}

	// Create the shader attributes

	GraphicsProgram::ShaderAttribute t1_data =
	{
		0,                                // layout
		GraphicsProgram::Types::FLOAT,  // Type
		3,                              // nb components per value
		false,                          // Should data be normalized?
		sizeof(t1),                    // data size
		&m_vertices[0]                // data
	};

	GraphicsProgram::ShaderAttribute c1_data =
	{
		1,                                      // layout
		GraphicsProgram::Types::FLOAT,        // Type
		3,                                    // nb components per value
		false,                                // Should data be normalized?
		m_colors.size()*sizeof(m_colors[0]), // data size
		&m_colors[0]                        // data
	};

	// Add them to the factory
	m_shaderAttributes.push_back(t1_data);
	m_shaderAttributes.push_back(c1_data);

	// Set shader names
	m_vertexShaderName   = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/Display.vert";
	m_fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PassThrough.frag";
}