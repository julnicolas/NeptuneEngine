#include "Graphics/Factories/PlanFactory.h"
#include "Graphics/VAOView.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/UniformVarNames.h"
#include "Graphics/GraphicalEnumMappingFunctions.h"
#include "Debug/NeptuneDebug.h"

using namespace Neptune;

PlanFactory::PlanFactory()
{
	initData({0.5f,0.5f,0.5f, 1.0f});
}

PlanFactory::PlanFactory(const Color& color)
{
	initData(color);
}

PlanFactory::PlanFactory(Texture* texture)
{
	if(texture != nullptr)
	{
		initData(texture);
	}
	else // Error: Wrong texture
	{
		NEP_LOG("Error: PlanFactory::PlanFactory(Texture*): Wrong parameter");
		initData(texture);
	}
}

VAOView* PlanFactory::create()
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
		v->getTransform().getDataPtr());

	// Create the triangle's renderer
	Renderer& renderer = v->getRenderer();
	renderer.setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLE_STRIP);
	renderer.setNbverticesToRender(4);

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
		pgm.addUniformVariable(mv);
		pgm.build();
	}

	return v;
}

void PlanFactory::initData(const Color& _color)
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
	m_vertices.push_back(0.5f);
	m_vertices.push_back(0.5f);
	m_vertices.push_back(0.0f);

	// Set color data
	m_colors.push_back(_color);
	m_colors.push_back(_color);
	m_colors.push_back(_color);
	m_colors.push_back(_color);

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

	// Add them to the factory
	m_shaderAttributes.push_back(t1_data);
	m_shaderAttributes.push_back(c1_data);

	// Set shader names
	m_vertexShaderName   = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/Display.vert";
	m_fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PassThrough.frag";
}

void PlanFactory::initData(Texture* texture)
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
	m_vertices.push_back(0.5f);
	m_vertices.push_back(0.5f);
	m_vertices.push_back(0.0f);

	//Set texture coordinates
	m_texCoords.push_back(0.0f); m_texCoords.push_back(1.0f);
	m_texCoords.push_back(1.0f); m_texCoords.push_back(1.0f);
	m_texCoords.push_back(0.0f); m_texCoords.push_back(0.0f);
	m_texCoords.push_back(1.0f); m_texCoords.push_back(0.0f);

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
	if(!m_texture->isInitialized())
		m_texture->init();
}