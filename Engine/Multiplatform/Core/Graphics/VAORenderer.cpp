#include "Graphics/VAORenderer.h"
#include "Graphics/IncludeOpenGL.h"
#include "Debug/NeptuneDebug.h"
#include "Graphics/GraphicalEnumMappingFunctions.h"

using namespace Neptune;

VAORenderer::VAORenderer()
{
	glGenVertexArrays( 1, &m_vao );
}

VAORenderer::~VAORenderer()
{
	glDeleteVertexArrays( 1, &m_vao );
}

bool VAORenderer::init()
{
	NEP_ASSERT( Renderer::init() );
	
	// First reading through the programs to get the one that uses the biggest number of parameters
	size_t biggest_nb_pms = 0;
	
	ConstGraphicalProgramIterator it_end = m_programs.cend();
	for ( ConstGraphicalProgramIterator it = m_programs.cbegin(); it != it_end; ++it )
		biggest_nb_pms = ( biggest_nb_pms < (*it)->getNbVertexAttributes() ) ? (*it)->getNbVertexAttributes() : biggest_nb_pms;

	// Bind the VAO 
	glBindVertexArray( m_vao );
	NEP_GRAPHICS_ASSERT();

	// Allocate the VBOs to store the vertex attributes and fill them
	GLuint* vbos_handle = new GLuint[ biggest_nb_pms ];

	// Iterate over the programs to set their parameter data
	for ( ConstGraphicalProgramIterator it = m_programs.cbegin(); it != it_end; ++it )
	{
		// Allocate the program's VBOs
		const u8 nb_attribs = (*it)->getNbVertexAttributes();
		glGenBuffers(nb_attribs,vbos_handle);

		// Populate them and enable them to be used in the graphical pipeline
		u8 attrib_index = 0;
		GraphicsProgram::ConstShaderAttributeIterator att_end = (*it)->shaderAttributeCEnd();
		for ( GraphicsProgram::ConstShaderAttributeIterator att = (*it)->shaderAttributeCBegin(); att != att_end; ++att, attrib_index++ )
		{
			// Fill the buffers
			glBindBuffer( GL_ARRAY_BUFFER, vbos_handle[ attrib_index ] );
			NEP_GRAPHICS_ASSERT();

			glBufferData( GL_ARRAY_BUFFER, att->m_size, att->m_data, GL_STATIC_DRAW );
			NEP_GRAPHICS_ASSERT();
			m_vbos[ &(**it) ].push_back( vbos_handle[ attrib_index ] );

			// Enable the shader's input interfaces
			glEnableVertexAttribArray( att->m_layout );
			NEP_GRAPHICS_ASSERT();
		}
	}
	delete[] vbos_handle;

	return true;
}

void VAORenderer::terminate()
{
	Renderer::terminate();
}

void VAORenderer::draw()
{
	glDrawArrays( MapDrawingPrimitive( m_drawingPrimitive ) , 0, m_nbverticesToRender );
	NEP_GRAPHICS_ASSERT();
}

void VAORenderer::bindShaderAttributes(const GraphicsProgram& pgm)
{
	glBindVertexArray( m_vao );
	Renderer::bindShaderAttributes( pgm );
}