#include "Renderer.h"
#include "Debug/NeptuneDebug.h"
#include <cstring>

#include "Graphics/IncludeOpenGL.h"

using namespace Neptune;

// U P D A T A B L E   I N T E R F A C E

bool Renderer::init()
{
	return true;
}

bool Renderer::update()
{
	// Browse the programs
	std::vector<GraphicalProgram>::const_iterator it_end = m_programs.cend();
	for(std::vector<GraphicalProgram>::const_iterator it = m_programs.cbegin(); it != it_end; ++it)
	{
		glUseProgram( it->getId() );
		bindUniformVars( it );
		bindShaderAttributes( *it );
		draw();
	}

	return true;
}

void Renderer::terminate()
{
	
}


// R E N D E R E R ' S   M E T H O D S


Neptune::Renderer::Renderer():
	 m_nbverticesToRender(0)
{
	setBackgroundColor( 0.0f, 0.0f, 0.0f, 1.0f );
}

Neptune::Renderer::~Renderer()
{

}

GraphicalProgram& Renderer::createProgram()
{
	size_t i = m_programs.size();
	m_programs.emplace_back();

	return m_programs[i];
}

static void SetSingleValuedUniform(s32 location, const GraphicalProgram::UniformVarInput& var)
{
	switch( var.getDataType() )
	{
	case GraphicalProgram::Types::FLOAT:
		glUniform1f( location, *(float*) var.getData() );
		break;

	case GraphicalProgram::Types::U32:
		glUniform1ui( location, *(u32*) var.getData() );
		break;

	case GraphicalProgram::Types::S32:
		glUniform1i( location, *(s32*) var.getData() );
		break;

	default:
		NEP_ASSERT( false );
		break;
	}
}

static void SetVectorialUniform(s32 location, const GraphicalProgram::UniformVarInput& var)
{
	switch( var.getDataType() )
	{

	////////////////////////////////////////////////
	case GraphicalProgram::Types::FLOAT:
		
		switch ( var.getNbRows() )
		{
		case 2:
			glUniform2fv( location, 1, (float*) var.getData() );
			break;

		case 3:
			glUniform3fv( location, 1, (float*) var.getData() );
			break;

		case 4:
			glUniform4fv( location, 1, (float*) var.getData() );
			break;

		default:
			NEP_ASSERT( false );
			break;
		}

		break;
	/////////////////////////////////////////////////


	///////////////////////////////////////////////////
	case GraphicalProgram::Types::U32:

		switch(var.getNbRows())
		{
		case 2:
			glUniform2uiv( location, 1, (u32*) var.getData() );
			break;			 	       		  				 
							 	       		  				 
		case 3:				 	       		  				 
			glUniform3uiv( location, 1, (u32*) var.getData() );
			break;			 	       		  				 
							 	       		  				 
		case 4:				 	       		  				 
			glUniform4uiv( location, 1, (u32*) var.getData() );
			break;

		default:
			NEP_ASSERT(false);
			break;
		}

		break;
	////////////////////////////////////////////////////

		
	////////////////////////////////////////////////////
	case GraphicalProgram::Types::S32:

		switch(var.getNbRows())
		{
		case 2:
			glUniform2iv(location,1,(s32*)var.getData());
			break;

		case 3:
			glUniform3iv(location,1,(s32*)var.getData());
			break;

		case 4:
			glUniform4iv(location,1,(s32*)var.getData());
			break;

		default:
			NEP_ASSERT(false);
			break;
		}

		break;
	///////////////////////////////////////////////////
	
	
	default:
		NEP_ASSERT( false );
		break;
	}
}

static void SetMatrixUniform(s32 location, const GraphicalProgram::UniformVarInput& var)
{
	NEP_ASSERT( var.getDataType() == GraphicalProgram::Types::FLOAT );

	if ( var.getNbRows() == var.getNbColumns() ) // Square matrix
	{
		switch ( var.getNbColumns() )
		{
		case 2:
			glUniformMatrix2fv( location, 1, false, (float*) var.getData() );
			break;

		case 3:
			glUniformMatrix3fv( location, 1, false, (float*) var.getData() );
			break;

		case 4:
			glUniformMatrix4fv( location, 1, false, (float*) var.getData() );
			break;
		}
	}
	else
	{
		switch ( var.getNbColumns() )
		{
		case 2:
			var.getNbRows() == 3 ? 
				glUniformMatrix2x3fv(location, 1, false, (float*) var.getData()) : 
				glUniformMatrix2x4fv(location, 1, false, (float*) var.getData());
			break;

		case 3:
			var.getNbRows() == 2 ? 
				glUniformMatrix3x2fv(location, 1, false, (float*) var.getData()) : 
				glUniformMatrix3x4fv(location, 1, false, (float*) var.getData());
			break;

		case 4:
			var.getNbRows() == 2 ? 
				glUniformMatrix4x2fv(location, 1, false, (float*) var.getData()) : 
				glUniformMatrix4x3fv(location, 1, false, (float*) var.getData());
			break;
		}
	}
}

static void SetUniform(s32 location, const GraphicalProgram::UniformVarInput& var)
{
	// First check if the data is a single value, a vector or a matrix

	if ( var.getNbRows() > 1 ) // It's either a vector or a matrix
	{
		if ( var.getNbColumns() > 1 ) // It's a vector
		{
			SetVectorialUniform( location, var );
		}
		else // It's a matrix
		{
			SetMatrixUniform( location, var );
		}
	}
	else // It's a single value
	{
		SetSingleValuedUniform( location, var );
	}
}

void Renderer::bindUniformVars(ConstGraphicalProgramIterator& it)
{
	// Browse the uniform vars
	GraphicalProgram::ConstUniformVarIterator uni_it_end = it->uniformVarCEnd();
	for(GraphicalProgram::ConstUniformVarIterator uni_it = it->uniformVarCBegin(); uni_it != uni_it_end; ++uni_it)
	{
		s32 location = glGetUniformLocation(it->getId(),uni_it->getName());
		NEP_ASSERT(location >= 0 && glGetError() != GL_INVALID_VALUE && glGetError() != GL_INVALID_OPERATION);

		SetUniform(location,*uni_it);
		//glUniformMatrix4fv(location,1,GL_FALSE,&m_mvMatrix[0][0]);
	}
}

// Old interface

void Neptune::Renderer::setBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_backgroundColor[0] = r; m_backgroundColor[1] = g;
	m_backgroundColor[2] = b; m_backgroundColor[3] = a;
}

/*void Neptune::Renderer::draw()
{
	// Screen cleaning
	glClearBufferfv(GL_COLOR, 0, m_backgroundColor);
	glClear( GL_DEPTH_BUFFER_BIT );

	// Prepare vertex shader
	//glBindVertexArray(m_vertexArrayObject);
	glUseProgram(m_renderPgm);
	sendCTMMatrix();

	// Draw-call
	//glDrawElements(GL_TRIANGLES, m_nbverticesToRender, m_vertexIndicesType, 0); // Watch out! the third value corresponds to the vertex-index type
	//switch ( glGetError() )
	//{
	//case GL_NO_ERROR:
	//	NEP_LOG("All good");
	//	break;
	//
	//case GL_INVALID_ENUM:
	//	NEP_LOG("Invalid enum");
	//	break;
	//
	//case GL_INVALID_VALUE:
	//	NEP_LOG("invalid value");
	//break;
	//
	//case GL_INVALID_OPERATION:
	//	NEP_LOG("invalid op");
	//	break;
	//
	//case GL_INVALID_FRAMEBUFFER_OPERATION:
	//	NEP_LOG("invalid fb op");
	//	break;
	//
	//case GL_OUT_OF_MEMORY:
	//	NEP_LOG("Out of mem");
	//break;
	//
	//default:
	//	NEP_LOG("WTF");
	//	break;
	//}
	
	
	glDrawArrays( GL_TRIANGLES, 0, m_nbverticesToRender ); // GL_TRIANGLES without any tess shader
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Only draws the edges
}*/