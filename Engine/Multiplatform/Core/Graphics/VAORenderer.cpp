#include "Graphics/VAORenderer.h"
#include "Graphics/IncludeOpenGL.h"
#include "Debug/NeptuneDebug.h"

using namespace Neptune;

VAORenderer::VAORenderer()
{
	glGenVertexArrays( 1, &m_vao );
}

VAORenderer::~VAORenderer()
{
	glDeleteVertexArrays( 1, &m_vao );

	// Free any allocated VRAM 
	GraphicalProgramIterator it_end = m_programs.end();
	for ( GraphicalProgramIterator it = m_programs.begin(); it != it_end; ++it ) // Browse every program
	{
		// Look through the vertex attributes
		std::vector<u32>& vbo = m_vbos[ &(**it) ];
		size_t vbo_size = vbo.size();

		for ( size_t i = 0; i < vbo_size; i++ )
			glDeleteBuffers( 1, &vbo[i] );
	}
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
		GraphicalProgram::ConstShaderAttributeIterator att_end = (*it)->shaderAttributeCEnd();
		for ( GraphicalProgram::ConstShaderAttributeIterator att = (*it)->shaderAttributeCBegin(); att != att_end; ++att, attrib_index++ )
		{
			// Fill the buffers
			glBindBuffer( GL_ARRAY_BUFFER, vbos_handle[ attrib_index ] );
			glBufferData( GL_ARRAY_BUFFER, att->m_size, att->m_data, GL_STATIC_DRAW );
			m_vbos[ &(**it) ].push_back( vbos_handle[ attrib_index ] );

			// Enable the shader's input interfaces
			glEnableVertexAttribArray( att->m_layout );
		}
	}
	delete[] vbos_handle;

	return true;
}

void VAORenderer::terminate()
{
	Renderer::terminate();
}

static GLenum MapDrawingPrimitive(Renderer::DrawingPrimitive const p)
{
	GLenum primitive = GL_INVALID_ENUM;

	switch ( p )
	{
	case Renderer::TRIANGLES:
		primitive = GL_TRIANGLES;
		break;
	}

	return primitive;
}

void VAORenderer::draw()
{
	glDrawArrays( MapDrawingPrimitive( m_drawingPrimitive ) , 0, m_nbverticesToRender );
}

static GLenum MapType(const GraphicalProgram::Types type)
{
	GLenum gl_type = GL_INVALID_ENUM;
	
	switch ( type )
	{
	case GraphicalProgram::Types::U32:
		gl_type = GL_UNSIGNED_INT;
		break;

	case GraphicalProgram::Types::S32:
		gl_type = GL_INT;
		break;

	case GraphicalProgram::Types::FLOAT:
		gl_type = GL_FLOAT;
		break;
	}

	return gl_type;
}

void VAORenderer::bindShaderAttributes(const GraphicalProgram& pgm)
{
	glBindVertexArray( m_vao );

	// Bind every pgm's vertex attribute to its vbo
	u32 i = 0;
	GraphicalProgram::ConstShaderAttributeIterator att_end = pgm.shaderAttributeCEnd();
	for(GraphicalProgram::ConstShaderAttributeIterator att = pgm.shaderAttributeCBegin(); att != att_end; ++att, i++)
	{
		glBindBuffer( GL_ARRAY_BUFFER, m_vbos[&pgm][i] );
		glVertexAttribPointer( att->m_layout, att->m_nbComponents, MapType( att->m_type ), GL_FALSE, 0, NULL );
	}
}