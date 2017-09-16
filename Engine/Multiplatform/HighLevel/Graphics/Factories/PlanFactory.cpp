#include "Graphics/Factories/PlanFactory.h"
#include "Graphics/VAOView.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/UniformVarNames.h"
#include "Graphics/GraphicalEnumMappingFunctions.h"
#include "Debug/NeptuneDebug.h"

using namespace Neptune;

PlanFactory::PlanFactory(const Color& _color):
	PlanFactory()
{
	// Add vertex shader
	const char VERTEX_SHADER_NAME[] = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/Display.vert";
	Shader vert(VERTEX_SHADER_NAME, GL_VERTEX_SHADER);
	m_program.add(vert.getId());
	
	// Add fragment shader
	const char FRAGMENT_SHADER_NAME[] = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PassThrough.frag";
	Shader frag(FRAGMENT_SHADER_NAME, GL_FRAGMENT_SHADER);
	m_program.add(frag.getId());
	
	// Set color
	m_colors.push_back(_color);
	m_colors.push_back(_color);
	m_colors.push_back(_color);
	m_colors.push_back(_color);

	// Create shader attribute
	GraphicsProgram::ShaderAttribute color_data =
	{
		1,                                       // layout
		GraphicsProgram::Types::FLOAT,          // Type
		4,                                     // nb components per value
		false,                                // Should data be normalized?
		m_colors.size()*sizeof(m_colors[0]), // data size
		&m_colors[0]                        // data
	};

	m_shaderAttributes.push_back(color_data);

	// Add color to the program
	m_program.addShaderAttribute(m_shaderAttributes[1]);
}

PlanFactory::PlanFactory(const char* _texturePath):
	PlanFactory()
{
	NEP_ASSERT(_texturePath != nullptr); // Invalid path
	
	// Add vertex shader
	const char VERTEX_SHADER_NAME[] = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/BasicDisplay.vert";
	Shader vert(VERTEX_SHADER_NAME, GL_VERTEX_SHADER);
	m_program.add(vert.getId());
	
	// Add fragment shader
	const char FRAGMENT_SHADER_NAME[] = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/ApplyTexture.frag";
	Shader frag(FRAGMENT_SHADER_NAME, GL_FRAGMENT_SHADER);
	m_program.add(frag.getId());

	// Set up texture
	m_texture.setPath(_texturePath);
	m_texture.init();

	// Add the texture to the program
	m_program.setTexture(&m_texture);

	// Create texture map data
	m_texCoords.push_back(0.0f); m_texCoords.push_back(1.0f);
	m_texCoords.push_back(1.0f); m_texCoords.push_back(1.0f);
	m_texCoords.push_back(0.0f); m_texCoords.push_back(0.0f);
	m_texCoords.push_back(1.0f); m_texCoords.push_back(0.0f);

	// Create shader attribute
	GraphicsProgram::ShaderAttribute tex_data =
	{
		1,                                              // layout
		GraphicsProgram::Types::FLOAT,                 // Type
		2,                                            // nb components per value
		false,                                       // Should data be normalized?
		m_texCoords.size()*sizeof(m_texCoords[0]),  // data size
		&m_texCoords[0]                            // data
	};

	m_shaderAttributes.push_back(tex_data);

	// Add the attribute to the program
	m_program.addShaderAttribute(m_shaderAttributes[1]);
}

VAOView* PlanFactory::create()
{
	VAOView* v = new VAOView;

	// Add the MV matrix
	GraphicsProgram::UniformVarInput mv(NEP_UNIVNAME_MV_MATRIX,
		GraphicsProgram::FLOAT,
		4,
		4,
		16*sizeof(float),
		v->getTransform().getDataPtr());

	m_program.addUniformVariable(mv);

	// Build the program
	m_program.build();

	// Set up the triangle's renderer
	v->setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLE_STRIP);
	v->setNbVerticesToRender(4);
	v->addGraphicsProgram(&m_program);

	return v;
}

// Private constructor
PlanFactory::PlanFactory()
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

	// Shader attributes
	GraphicsProgram::ShaderAttribute pos_data =
	{
		0,                                            // layout
		GraphicsProgram::Types::FLOAT,               // Type
		3,                                          // nb components per value
		false,                                     // Should data be normalized?
		m_vertices.size()*sizeof(m_vertices[0]),  // data size
		&m_vertices[0]                           // data
	};

	m_shaderAttributes.push_back(pos_data);

	// Add position to the program
	m_program.addShaderAttribute(m_shaderAttributes[0]);
}