#include "Graphics/Factories/CubeFactory.h"
#include "Graphics/VAOView.h"
#include "Graphics/Shader.h"
#include "Graphics/UniformVarNames.h"
#include "Graphics/Texture.h"
#include "Debug/NeptuneDebug.h"

using namespace Neptune;

CubeFactory::CubeFactory()
{
	initCubeData({0.5f,0.5f,0.5f,1.0f});
}

CubeFactory::CubeFactory(const Color& color)
{
	initCubeData( color );
}

CubeFactory::CubeFactory(Texture* texture)
{
	initCubeData(texture);
}

VAOView* CubeFactory::create()
{
	VAOView* v = new VAOView;

	// Add the texture if one has been provided
	if(m_texture != nullptr)
		v->bindTexture(m_texture);
	
	// Add the MV matrix
	GraphicsProgram::UniformVarInput mv(NEP_UNIVNAME_MV_MATRIX,
		GraphicsProgram::FLOAT,
		4,
		4,
		16*sizeof(float),
		v->getTransform().getDataPtr() );

	// Create the renderer
	Renderer& renderer = v->getRenderer();

	renderer.setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLES);
	renderer.setNbverticesToRender(NB_VERTICES_TO_RENDER);

	// Create the shaders to display the cube
	Shader vert(m_vertexShaderName.c_str(),GL_VERTEX_SHADER);
	Shader frag(m_fragmentShaderName.c_str(),GL_FRAGMENT_SHADER);

	// Create the program to display a cube
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

void CubeFactory::initCubeData(const Color& _color)
{
	// Set vertex data
	float t1[] =
	{
		// Bottom
		-0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.5f,-0.5f,0.5f,

		// Front
		-0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,0.5f,-0.5f,
		0.5f,0.5f,-0.5f,

		// Right
		0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,0.5f,
		0.5f,0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,0.5f,-0.5f,
		0.5f,0.5f,0.5f,

		// Back
		-0.5f,-0.5f,0.5f,
		0.5f,-0.5f,0.5f,
		0.5f,0.5f,0.5f,
		-0.5f,-0.5f,0.5f,
		-0.5f,0.5f,0.5f,
		0.5f,0.5f,0.5f,

		// Left
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		-0.5f,0.5f,0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,0.5f,-0.5f,
		-0.5f,0.5f,0.5f,

		// Top
		-0.5f,0.5f,-0.5f,
		0.5f,0.5f,-0.5f,
		0.5f,0.5f,0.5f,
		-0.5f,0.5f,-0.5f,
		-0.5f,0.5f,0.5f,
		0.5f,0.5f,0.5f
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
	m_vertexShaderName   = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/Display.vert";
	m_fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PassThrough.frag";
}

void CubeFactory::initCubeData(Texture* texture)
{
	// Set vertex data
	float t1[] =
	{
		// Bottom
		-0.5f, -0.5f,-0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,

		// Front
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,

		// Right
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,

		// Back
		-0.5f,-0.5f,0.5f,
		0.5f,-0.5f,0.5f,
		0.5f,0.5f,0.5f,
		-0.5f,-0.5f,0.5f,
		-0.5f,0.5f,0.5f,
		0.5f,0.5f,0.5f,

		// Left
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		-0.5f,0.5f,0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,0.5f,-0.5f,
		-0.5f,0.5f,0.5f,

		// Top
		-0.5f,0.5f,-0.5f,
		0.5f,0.5f,-0.5f,
		0.5f,0.5f,0.5f,
		-0.5f,0.5f,-0.5f,
		-0.5f,0.5f,0.5f,
		0.5f,0.5f,0.5f

	};
	for(unsigned int i = 0; i < sizeof(t1)/sizeof(float); i++)
		m_vertices.push_back(t1[i]);

	// Set texture coordinate data
	float coords[12];
	coords[0]  = 0.0f; coords[1] = 1.0f; 
	coords[2]  = 1.0f; coords[3] = 1.0f;
	coords[4]  = 1.0f; coords[5] = 0.0f;

	coords[6]  = 0.0f; coords[7]  = 1.0f;
	coords[8]  = 0.0f; coords[9]  = 0.0f;
	coords[10] = 1.0f; coords[11] = 0.0f;
	

	for(unsigned int i = 0; i < NB_VERTICES_TO_RENDER/6; i++)
	{
		for ( u8 j = 0; j < 12; j++ )
			m_texCoords.push_back(coords[j]);
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
		1,                                             // layout
		GraphicsProgram::Types::FLOAT,                // Type
		2,                                           // nb components per value
		false,                                      // Should data be normalized?
		m_texCoords.size()*sizeof(m_texCoords[0]), // data size
		&m_texCoords[0]                           // data
	};

	// Add them to the factory
	m_shaderAttributes.push_back(t1_data);
	m_shaderAttributes.push_back(c1_data);

	// Set shader names
	m_vertexShaderName   = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/BasicDisplay.vert";
	m_fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/ApplyTexture.frag";

	// Init texture if necessary
	m_texture = texture;
	if(!m_texture->isInitialized())
		m_texture->init();
}