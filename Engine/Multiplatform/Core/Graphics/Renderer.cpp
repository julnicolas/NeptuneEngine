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
	NEP_ASSERT( m_vboHandles == nullptr); // Error object is already initialized
	
	// First reading through the programs to get the one that uses the biggest number of parameters
	size_t biggest_nb_pms = 0;
	
	ConstGraphicalProgramIterator it_end = m_programs.cend();
	for ( ConstGraphicalProgramIterator it = m_programs.cbegin(); it != it_end; ++it )
		biggest_nb_pms = ( biggest_nb_pms < (*it)->getNbVertexAttributes() ) ? (*it)->getNbVertexAttributes() : biggest_nb_pms;

	// Allocate the VBOs to store the vertex attributes and fill them
	m_vboHandles = new u32[ biggest_nb_pms ];

	// Iterate over the programs to set their parameter data
	for ( ConstGraphicalProgramIterator it = m_programs.cbegin(); it != it_end; ++it )
	{
		// Allocate the program's VBOs
		const u8 nb_attribs = (*it)->getNbVertexAttributes();
		glGenBuffers(nb_attribs,m_vboHandles);

		// Populate them and enable them to be used in the graphical pipeline
		u8 attrib_index = 0;
		GraphicsProgram::ConstShaderAttributeIterator att_end = (*it)->shaderAttributeCEnd();
		for ( GraphicsProgram::ConstShaderAttributeIterator att = (*it)->shaderAttributeCBegin(); att != att_end; ++att, attrib_index++ )
		{
			// Fill the buffers
			glBindBuffer( GL_ARRAY_BUFFER, m_vboHandles[ attrib_index ] );
			NEP_GRAPHICS_ASSERT();

			glBufferData( GL_ARRAY_BUFFER, att->m_size, att->m_data, GL_STATIC_DRAW );
			NEP_GRAPHICS_ASSERT();
			m_vbos[ &(**it) ].push_back( m_vboHandles[ attrib_index ] );

			// Enable the shader's input interfaces
			glEnableVertexAttribArray( att->m_layout );
			NEP_GRAPHICS_ASSERT();
		}
	}

	return true;
}

bool Renderer::cloneInit(const	Renderer& _source)
{
	// Object's copy is done in subclass

	// Iterate over the programs to set their parameter data
	ConstGraphicalProgramIterator it_end = m_programs.cend();
	for ( ConstGraphicalProgramIterator it = m_programs.cbegin(); it != it_end; ++it )
	{
		// Populate them and enable them to be used in the graphical pipeline
		u8 attrib_index = 0;
		GraphicsProgram::ConstShaderAttributeIterator att_end = (*it)->shaderAttributeCEnd();
		for ( GraphicsProgram::ConstShaderAttributeIterator att = (*it)->shaderAttributeCBegin(); att != att_end; ++att, attrib_index++ )
		{
			// Fill the buffers
			glBindBuffer( GL_ARRAY_BUFFER, m_vboHandles[ attrib_index ] );
			NEP_GRAPHICS_ASSERT();

			glBufferData( GL_ARRAY_BUFFER, att->m_size, att->m_data, GL_STATIC_DRAW );
			NEP_GRAPHICS_ASSERT();
			m_vbos[ &(**it) ].push_back( m_vboHandles[ attrib_index ] );

			// Enable the shader's input interfaces
			glEnableVertexAttribArray( att->m_layout );
			NEP_GRAPHICS_ASSERT();
		}
	}

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
	// Delete vbo handles
	delete[] m_vboHandles;
	m_vboHandles = nullptr;
	
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


// R E N D E R E R ' S   M E T H O D S


Neptune::Renderer::Renderer():
	 m_nbverticesToRender(0), m_vboHandles(nullptr)
{
	
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

// Can only check if an array is not outrageously long and takes
// up all the space reserved by uniforms.
static bool CheckArrayMaxSize(GLenum _shaderStage, u32 _size)
{
	GLint max_size = 0;
	glGetIntegerv(_shaderStage, &max_size);

	// REASON FOR MULTIPLYING BY 4
	// SOURCE : khronos.org
	// OpenGL implementations are allowed to reject shaders for 
	// implementation-dependent reasons. So you can have fewer active 
	// uniform components by your reckoning and still fail to link due 
	// to uniform limits. This is usually on hardware that is innately 
	// vector hardware. Pre-GeForce 8xxx hardware, and all ATi hardware
	// does this. In this case, you should assume that each separate 
	// uniform takes up 4 components, much like it would in D3D.
	return _size*4 < max_size; 
}

static void SetVectorOrArrayUniform(s32 location, const GraphicsProgram::UniformVarInput& var)
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

		// More than 4 values,  it is necessarily an array and cannot be represented by a vector in GLSL
		default:
			NEP_ASSERT(CheckArrayMaxSize(GL_MAX_VERTEX_UNIFORM_COMPONENTS, var.getNbRows()));
			NEP_ASSERT(CheckArrayMaxSize(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, var.getNbRows()));
			glUniform1fv( location, var.getNbRows(), (float*) var.getData() );
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

		// More than 4 values,  it is necessarily an array and cannot be represented by a vector in GLSL
		default:
			NEP_ASSERT(CheckArrayMaxSize(GL_MAX_VERTEX_UNIFORM_COMPONENTS, var.getNbRows()));
			NEP_ASSERT(CheckArrayMaxSize(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, var.getNbRows()));
			glUniform1uiv( location, var.getNbRows(), (u32*) var.getData() );
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

		// More than 4 values,  it is necessarily an array and cannot be represented by a vector in GLSL
		default:
			NEP_ASSERT(CheckArrayMaxSize(GL_MAX_VERTEX_UNIFORM_COMPONENTS, var.getNbRows()));
			NEP_ASSERT(CheckArrayMaxSize(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, var.getNbRows()));
			glUniform1iv( location, var.getNbRows(), (s32*) var.getData() );
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
	NEP_ASSERT( var.getNbColumns() > 0 && var.getNbColumns() <= 4 );

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

	if ( var.getNbRows() > 1 ) // It's either an array, a vector or a matrix
	{
		if ( var.getNbColumns() == 1 ) // It's a vector or an array
		{
			SetVectorOrArrayUniform( location, var );
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
	auto att_end = pgm.shaderAttributeCEnd();
	for(auto att = pgm.shaderAttributeCBegin(); att != att_end; ++att,i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER,m_vbos[&pgm][i]);
		glVertexAttribPointer(att->m_layout,att->m_nbComponents,MapType(att->m_type),att->m_normalized,0,NULL);

		NEP_GRAPHICS_ASSERT();
	}
}

void Renderer::bindTextures(GraphicsProgram& _pgm)
{
	auto tex_end = _pgm.textureEnd();
	for(auto tex = _pgm.textureBegin(); tex != tex_end; ++tex)
	{
		tex->second->update();
	}
}

void Renderer::bindUniformVars(ConstGraphicalProgramIterator& it)
{
	// Browse the uniform vars
	auto uni_it_end = (*it)->uniformVarCEnd();
	for(auto uni_it = (*it)->uniformVarCBegin(); uni_it != uni_it_end; ++uni_it)
	{
		s32 location = glGetUniformLocation( (*it)->getResourceID(), uni_it->second.getName() );
		
		NEP_GRAPHICS_ASSERT();
		NEP_ASSERT_ERR_MSG( location >= 0, 
			"Uniform variable %s not found or not used in shader %s (removed by the GLSL compiler)", 
			uni_it->second.getName(), (*it)->getDebugName() );
		
		SetUniform(location, uni_it->second);
	}
}