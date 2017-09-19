#include "Graphics/Factories/PolygonFactory.h"
#include "Graphics/View.h"
#include "Graphics/Shader.h"
#include "Graphics/UniformVarNames.h"

#include "Debug/NeptuneDebug.h"


using namespace Neptune;


PolygonFactory::PolygonFactory(const Color& _color)
{
	// The vertex shader is set in create() once it is known whether lighting is supported by the instantiated object or not.
	
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
	
	// The vertex shader is set in create() once it is known whether lighting is supported by the instantiated object or not.
	
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

	// Build the program
	m_program.build();

	// Add the program to the view
	v->addGraphicsProgram(&m_program);

	return v;
}

typedef float float_3_t[3];

static void CreateLightSource(GraphicsProgram& _program, float_3_t _diffuseLightDirection, float_3_t _diffuseLightColor)
{
	// Light direction
	_diffuseLightDirection[0] = 0.0f; _diffuseLightDirection[1] = 0.0f; _diffuseLightDirection[2] = 1.0f; // left handed, world frame

	GraphicsProgram::UniformVarInput diffuse_light_direction(NEP_UNIVNAME_DIFFUSE_LIGHT_DIRECTION,
		GraphicsProgram::FLOAT,
		3,
		1,
		3*sizeof(float),//_diffuseLightDirection),
		_diffuseLightDirection);

	_program.addUniformVariable(diffuse_light_direction);

	// Light's color
	_diffuseLightColor[0] = 1.0f; _diffuseLightColor[1] = 1.0f; _diffuseLightColor[2] = 1.0f; // white light
	
	GraphicsProgram::UniformVarInput diffuse_light_color_input(NEP_UNIVNAME_DIFFUSE_LIGHT_COLOR,
		GraphicsProgram::FLOAT,
		3,
		1,
		3*sizeof(float),//_diffuseLightColor),
		_diffuseLightColor);

	_program.addUniformVariable(diffuse_light_color_input);
}

void PolygonFactory::initPolygonData()
{
	// Add vertex data if needed
	if (m_vertices.empty()) // True if createVertexData() hasn't been called
	{
		//
		//		C R E A T E   V E R T I C E S
		//

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

		
		//
		//		U S E   C O L O R S   O R   T E X T U R E S
		//

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

		//
		//		C R E A T E   S I M P L E   L I G H T   S O U R C E
		//

		const char LIGHT_SOURCE_VERTEX_SHADER_NAME[] = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/DiffuseLight.vert";
		const char FALLBACK_VERTEX_SHADER_NAME[]     = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/Display.vert";
		
		createNormalData();
		
		if ( !m_normals.empty() )
		{
			// Simple light sources can be used, the appropriate vertex shader can be set
			Shader vert(LIGHT_SOURCE_VERTEX_SHADER_NAME, GL_VERTEX_SHADER);
			m_program.add(vert.getId());

			// Create light sources
			CreateLightSource(m_program, m_diffuseLightDirection, m_diffuseLightColor);

			// Create the normal parameter
			GraphicsProgram::ShaderAttribute n_data =
			{
				2,										// layout
				GraphicsProgram::Types::FLOAT,			// Type
				3,										// nb components per value
				false,									// Should data be normalized?
				m_normals.size()*sizeof(m_normals[0]),	// data size
				&m_normals[0]							// data
			};

			// Add normals to the program
			m_shaderAttributes.push_back(n_data);
		}
		else
		{
			// Light sources are not supported so a raw display shader is used
			Shader vert(FALLBACK_VERTEX_SHADER_NAME, GL_VERTEX_SHADER);
			m_program.add(vert.getId());

			NEP_LOG("Warning PolygonFactory::initPolygonData(): normal data are not implemented for polygon with address %x", this);
		}
	}
}