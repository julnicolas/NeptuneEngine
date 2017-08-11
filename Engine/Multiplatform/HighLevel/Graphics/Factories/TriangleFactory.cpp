#include "Graphics/Factories/TriangleFactory.h"
#include "Graphics/VAOView.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/UniformVarNames.h"
#include "Graphics/GraphicalEnumMappingFunctions.h"
#include "Debug/NeptuneDebug.h"

using namespace Neptune;

TriangleFactory::TriangleFactory()
{
	initData({0.5f,0.5f,0.5f,1.0f});
}

TriangleFactory::TriangleFactory(const Color& color)
{
	initData( color );
}

TriangleFactory::TriangleFactory(Texture* texture)
{
	if ( texture != nullptr )
	{
		initData( texture );
	}
	else // Error: Wrong texture
	{
		NEP_LOG("Error: TriangleFactory::TrinagleFactory(const char*): Wrong parameter");
		initData( texture );
	}
}

VAOView* TriangleFactory::create()
{
	VAOView* v = new VAOView;

	// Add the texture if one has been provided
	if ( m_texture != nullptr )
		v->bindTexture( m_texture );
	
	// Add the MV matrix

	GraphicsProgram::UniformVarInput mv(NEP_UNIVNAME_MV_MATRIX,
		GraphicsProgram::FLOAT,
		4,
		4,
		16*sizeof(float),
		v->getTransform().getDataPtr());

	// Debug - Light direction
	float* diffuse_light = new float[3];
	diffuse_light[0] = 0.0f; diffuse_light[1] = 0.0f; diffuse_light[2] = 1.0f; // left handed, world frame
	GraphicsProgram::UniformVarInput diffuse_light_direction("DiffuseLightDirection",
		GraphicsProgram::FLOAT,
		3,
		1,
		3*sizeof(float),
		diffuse_light);

	// Debug - light's color
	float* diffuse_light_color = new float[3];
	diffuse_light_color[0] = 0.0f; diffuse_light_color[1] = 1.0f; diffuse_light_color[2] = 0.0f; // green light
	GraphicsProgram::UniformVarInput diffuse_light_color_input("DiffuseLightColor",
		GraphicsProgram::FLOAT,
		3,
		1,
		3*sizeof(float),
		diffuse_light_color);

	// Create the triangle's renderer
	Renderer& renderer = v->getRenderer();
	renderer.setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLES);
	renderer.setNbverticesToRender(3);

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
		pgm.addShaderAttribute(m_shaderAttributes[2]);
		pgm.addUniformVariable(mv);
		pgm.addUniformVariable(diffuse_light_direction);
		pgm.addUniformVariable(diffuse_light_color_input);
		pgm.build();
	}

	return v;
}

void TriangleFactory::initData(const Color& _color)
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
	m_colors.push_back(_color);
	m_colors.push_back(_color);
	m_colors.push_back(_color);

	// Debug
	float* normals = new float[9];
	normals[0] = 0.0f; normals[1] = 0.0f; normals[2] = -1.0f;
	normals[3] = 0.0f; normals[4] = 0.0f; normals[5] = -1.0f;
	normals[6] = 0.0f; normals[7] = 0.0f; normals[8] = -1.0f;

	// Create the shader attributes

	GraphicsProgram::ShaderAttribute t1_data =
	{
		0,                                          // layout
		GraphicsProgram::Types::FLOAT,            // Type
		3,                                        // nb components per value
		false,                                    // Should data be normalized?
		m_vertices.size()*sizeof(m_vertices[0]), // data size
		&m_vertices[0]                          // data
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

	// DEBUG
	GraphicsProgram::ShaderAttribute n1_data =
	{
		2,                                      // layout
		GraphicsProgram::Types::FLOAT,        // Type
		3,                                    // nb components per value
		false,                                // Should data be normalized?
		9*sizeof(normals[0]),				// data size
		normals		                        // data
	};

	// Add them to the factory
	m_shaderAttributes.push_back(t1_data);
	m_shaderAttributes.push_back(c1_data);
	m_shaderAttributes.push_back(n1_data);

	// Set shader names
	m_vertexShaderName   = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/DiffuseLight.vert";
	m_fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PassThrough.frag";
}

void TriangleFactory::initData(Texture* texture)
{
	// Set vertex data
	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.5f);
	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.5f);
	m_vertices.push_back(0.0f);

	//Set texture coordinates
	m_texCoords.push_back( 0.0f ); m_texCoords.push_back( 1.0f );
	m_texCoords.push_back( 1.0f ); m_texCoords.push_back( 1.0f );
	m_texCoords.push_back( 0.0f ); m_texCoords.push_back( 0.0f );

	// Create shader attributes
	GraphicsProgram::ShaderAttribute pos_data =
	{
		0,                                            // layout
		GraphicsProgram::Types::FLOAT,               // Type
		3,                                          // nb components per value
		false,                                     // Should data be normalized?
		m_vertices.size()*sizeof(m_vertices[0]),  // data size
		&m_vertices[0]                           // data
	};

	GraphicsProgram::ShaderAttribute tex_data =
	{
		1,                                              // layout
		GraphicsProgram::Types::FLOAT,                 // Type
		2,                                            // nb components per value
		false,                                       // Should data be normalized?
		m_texCoords.size()*sizeof(m_texCoords[0]),  // data size
		&m_texCoords[0]                            // data
	};

	// Add them to the factory
	m_shaderAttributes.push_back(pos_data);
	m_shaderAttributes.push_back(tex_data);

	// Set shader names
	m_vertexShaderName   = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/BasicDisplay.vert";
	m_fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/ApplyTexture.frag";

	// Init texture if necessary
	m_texture = texture;
	if ( !m_texture->isInitialized() )
		m_texture->init();
}