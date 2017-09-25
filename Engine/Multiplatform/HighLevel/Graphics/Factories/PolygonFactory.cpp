#include "Graphics/Factories/PolygonFactory.h"
#include "Graphics/View.h"
#include "Graphics/Shader.h"
#include "Graphics/UniformVarNames.h"

#include "Debug/NeptuneDebug.h"


using namespace Neptune;

PolygonFactory::PolygonFactory(const Color& _color)
{
	// Add vertex shader
	const char VERTEX_SHADER_NAME[] = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/Display.vert";
	Shader vert(VERTEX_SHADER_NAME, GL_VERTEX_SHADER);
	m_program.add(vert.getId());
	
	// Add fragment shader
	const char FRAGMENT_SHADER_NAME[] = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PassThrough.frag";
	Shader frag(FRAGMENT_SHADER_NAME, GL_FRAGMENT_SHADER);
	m_program.add(frag.getId());

	// Add the color for the first vertex
	// Doing so let us know what color to use when creating the view
	m_colors.push_back(_color);
}

PolygonFactory::PolygonFactory(const char* _texturePath)
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
}

View* PolygonFactory::create()
{
	initPolygonData();

	// Create the view
	View* v = createViewAndSetUpRenderParameters();

	// Add the MV matrix
	GraphicsProgram::UniformVarInput mv(NEP_UNIVNAME_MV_MATRIX,
		GraphicsProgram::FLOAT,
		4,
		4,
		16*sizeof(float),
		v->getTransform().getDataPtr());

	m_program.addUniformVariable(mv);

	// Add a default projection
	Mat4 I;
	GraphicsProgram::UniformVarInput projection_matrix(NEP_UNIVNAME_PROJ_MATRIX,
		GraphicsProgram::FLOAT,
		4,
		4,
		16*sizeof(float),
		&I);

	m_program.addUniformVariable(projection_matrix);

	// Build the program
	m_program.build();

	// Add the program to the view
	v->addGraphicsProgram(&m_program);

	return v;
}

void PolygonFactory::initPolygonData()
{
	// Add vertex data if needed
	if (m_vertices.empty()) // True if createVertexData() hasn't been called
	{
		createVertexData();
		
		// Add shader attribute
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

		// Check if textures are used
		if (m_texture.getWidth() != 0 || m_texture.getHeight() != 0) // True if textures are used
		{
			createTextureCoordinates();

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
		else // Colors are used
		{
			// Set color
			{
				size_t nb_colors_to_set = m_vertices.size()/3 - 1; // As many color values as their are vertices and we set one in the constructor
				Color color = m_colors.back();

				for (size_t i = 0; i < nb_colors_to_set; i++)
					m_colors.push_back(color);			
			}

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
	}
}