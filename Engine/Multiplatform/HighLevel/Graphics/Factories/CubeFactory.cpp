#include "Graphics/Factories/CubeFactory.h"
#include "Graphics/VAOView.h"
#include "Graphics/Shader.h"
#include "Graphics/UniformVarNames.h"
#include "Debug/NeptuneDebug.h"

using namespace Neptune;

CubeFactory::CubeFactory()
{
	initCubeData( {0.5f, 0.5f, 0.5f, 1.0f} );
}

CubeFactory::CubeFactory(const Color& color)
{
	initCubeData( color );
}

VAOView* CubeFactory::create()
{
	VAOView* v = new VAOView;
	
	// Add the MV matrix
	GraphicsProgram::UniformVarInput mv(NEP_UNIVNAME_MV_MATRIX,
		GraphicsProgram::FLOAT,
		4,
		4,
		16*sizeof(float),
		v->getTransform().getDataPtr() );

	// Add the Ambient light source
	// First it is set to 0 since a 
	// view must be bound to a light 
	// source if it's within its reach
	GraphicsProgram::UniformVarInput ambientLight( NEP_UNIVNAME_AMBIENT_LIGHT,
		GraphicsProgram::FLOAT,
		3, // RGB value
		1, // Is a vec3 so only one column
		3*sizeof(float),
		nullptr	
	);


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
		pgm.addUniformVariable( ambientLight );
		pgm.build();
	}

	return v;
}

void CubeFactory::initCubeData(const Color& _color)
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
	for(unsigned int i = 0; i < NB_VERTICES_TO_RENDER; i++)
	{
		m_colors.push_back( _color );
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
		4,                                    // nb components per value
		false,                                // Should data be normalized?
		m_colors.size()*sizeof(m_colors[0]), // data size
		&m_colors[0]                        // data
	};

	// Add them to the factory
	m_shaderAttributes.push_back(t1_data);
	m_shaderAttributes.push_back(c1_data);

	// Set shader names
	m_vertexShaderName   = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/AmbientLight.vert";
	m_fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PassThrough.frag";
}