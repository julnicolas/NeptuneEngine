#include "Renderer.h"
#include "Graphics/GraphicalEnumMappingFunctions.h"
#include "Graphics/Texture.h"
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
	// Browse the programs and make a draw-call
	ConstGraphicalProgramIterator it_end = m_programs.cend();
	for(ConstGraphicalProgramIterator it = m_programs.cbegin(); it != it_end; ++it)
	{
		glUseProgram( (*it)->getResourceID() );
		NEP_GRAPHICS_ASSERT();

		bindUniformVars( it );
		bindShaderAttributes( **it );
		bindTextures( **it );
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
	
}

Neptune::Renderer::~Renderer()
{
	// Free the vbos (allocated VRAM)
	GraphicalProgramIterator it_end = m_programs.end();
	for(GraphicalProgramIterator it = m_programs.begin(); it != it_end; ++it) // Browse every program
	{
		// Look through the vertex attributes
		std::vector<u32>& vbo = m_vbos[&(**it)];
		size_t vbo_size = vbo.size();

		for(size_t i = 0; i < vbo_size; i++)
		{
			glDeleteBuffers(1,&vbo[i]);
			NEP_GRAPHICS_ASSERT();
		}
	}
}

Renderer::ProgramID Renderer::addProgram(GraphicsProgram* _pgm)
{
	ProgramID id = m_programs.size();
	m_programs.push_back(_pgm);

	return id;
}

bool Renderer::updateUniform(u8 pgm_index, const char* name, const void* data)
{
	GraphicsProgram::UniformVarIterator it = m_programs[pgm_index]->getUniformVar(name);
	
	NEP_ASSERT( it != m_programs[pgm_index]->uniformVarEnd() ); // Error, uniform couldn't be found
	
	if ( it != m_programs[pgm_index]->uniformVarEnd() )
	{
		it->second.setData( data );
		return true;
	}

	return false;
}

static void SetSingleValuedUniform(s32 location, const GraphicsProgram::UniformVarInput& var)
{
	switch( var.getDataType() )
	{
	case GraphicsProgram::Types::FLOAT:
		glUniform1f( location, *(float*) var.getData() );
		break;

	case GraphicsProgram::Types::U32:
		glUniform1ui( location, *(u32*) var.getData() );
		break;

	case GraphicsProgram::Types::S32:
		glUniform1i( location, *(s32*) var.getData() );
		break;

	default:
		NEP_ASSERT( false );
		break;
	}

	NEP_GRAPHICS_ASSERT();
}

static void SetVectorialUniform(s32 location, const GraphicsProgram::UniformVarInput& var)
{
	switch( var.getDataType() )
	{

	////////////////////////////////////////////////
	case GraphicsProgram::Types::FLOAT:
		
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
	case GraphicsProgram::Types::U32:

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
	case GraphicsProgram::Types::S32:

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

	NEP_GRAPHICS_ASSERT();
}

static void SetMatrixUniform(s32 location, const GraphicsProgram::UniformVarInput& var)
{
	NEP_ASSERT( var.getDataType() == GraphicsProgram::Types::FLOAT );

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

	NEP_GRAPHICS_ASSERT();
}

static void SetUniform(s32 location, const GraphicsProgram::UniformVarInput& var)
{
	// First check if the data is a single value, a vector or a matrix

	if ( var.getNbRows() > 1 ) // It's either a vector or a matrix
	{
		if ( var.getNbColumns() == 1 ) // It's a vector
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

void Renderer::bindShaderAttributes(const GraphicsProgram& pgm)
{
	// Bind every pgm's vertex attribute to its vbo
	u32 i = 0;
	GraphicsProgram::ConstShaderAttributeIterator att_end = pgm.shaderAttributeCEnd();
	for(GraphicsProgram::ConstShaderAttributeIterator att = pgm.shaderAttributeCBegin(); att != att_end; ++att,i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER,m_vbos[&pgm][i]);
		glVertexAttribPointer(att->m_layout,att->m_nbComponents,MapType(att->m_type),att->m_normalized,0,NULL);

		NEP_GRAPHICS_ASSERT();
	}
}

void Renderer::bindTextures(const GraphicsProgram& _pgm)
{
	u32 i = 0;
	GraphicsProgram::ConstTextureIterator tex_end = _pgm.textureCEnd();
	for(GraphicsProgram::ConstTextureIterator tex = _pgm.textureCBegin(); tex != tex_end; ++tex,i++)
	{
		(*tex)->update();
	}
}

void Renderer::bindUniformVars(ConstGraphicalProgramIterator& it)
{
	// Browse the uniform vars
	GraphicsProgram::ConstUniformVarIterator uni_it_end = (*it)->uniformVarCEnd();
	for(GraphicsProgram::ConstUniformVarIterator uni_it = (*it)->uniformVarCBegin(); uni_it != uni_it_end; ++uni_it)
	{
		s32 location = glGetUniformLocation( (*it)->getResourceID(), uni_it->second.getName() );
		
		NEP_GRAPHICS_ASSERT();
		NEP_ASSERT(location >= 0); // Uniform variable not found or not used in the shader (removed by the GLSL compiler)

		SetUniform(location, uni_it->second);
	}
}