#include "GraphicalProgram.h"
#include "Debug/NeptuneDebug.h"
#include <GL/glew.h>

using namespace Neptune;

GraphicalProgram::GraphicalProgram():
	m_program(0)
{
	m_program = glCreateProgram();
}

GraphicalProgram::~GraphicalProgram()
{
	// Destroy the uniform variables
	{
		std::vector<UniformVarInput>::iterator it_end = m_uniformVars.end();
		for(std::vector<UniformVarInput>::iterator it = m_uniformVars.begin(); it != it_end; ++it)
			it->destruct();
	}
	
	// Destroy the uniform blocks
	{
		std::map<u32, u8*>::iterator it_end = m_uniform_block_buffers.end();
		for (std::map<u32, u8*>::iterator it = m_uniform_block_buffers.begin(); it != it_end; ++it)
			delete[] it->second;
	}
}

void GraphicalProgram::add(u32 shader)
{
	glAttachShader(m_program, shader);
}

bool GraphicalProgram::build()
{
	glLinkProgram(m_program);

	return true;
}

u32 GraphicalProgram::addUniformBlock(const char* block_name, const char** variables_name, const UniformBlockData* values, const u32 size)
{
	
    // Get the index of the uniform block
    u32 blockIndex = glGetUniformBlockIndex(m_program, block_name);

    // Allocate space for the buffer
    s32 blockSize;
    glGetActiveUniformBlockiv(m_program, blockIndex,
                              GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	
	u8* blockBuffer = new u8[blockSize];

    // Query the offsets of each block variable
    u32* indices = new u32[size];
    glGetUniformIndices(m_program, size, variables_name, indices);
    
    s32* offset = new s32[size];
    glGetActiveUniformsiv(m_program, size, indices, GL_UNIFORM_OFFSET, offset);

    // Store data within the buffer at the appropriate offsets
    for (u32 i = 0; i < size; i++)
		memcpy(blockBuffer + offset[i], values[i].m_data, values[i].m_size);
	

    // Create the buffer object and copy the data
    u32 uboHandle;
    glGenBuffers( 1, &uboHandle );
    glBindBuffer( GL_UNIFORM_BUFFER, uboHandle );			// Bind to modify the buffer
    glBufferData( GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW );

    // Bind the buffer object to the uniform block
    glBindBufferBase( GL_UNIFORM_BUFFER, 1, uboHandle );	// Bind to use as a source parameter in the shader
    
    // We don't need this if we specify the binding within the shader
	// glUniformBlockBinding(programHandle, blockIndex, 0);

	// Cleaning out 

	delete[] indices;
	delete[] offset;

	// Add the new buffer to the program list of uniform buffers

	m_uniform_block_buffers[uboHandle] = blockBuffer;
	return uboHandle;
}

void GraphicalProgram::rmUniformBlock(const u32 ubo_handle)
{
	delete[] m_uniform_block_buffers[ubo_handle];
	m_uniform_block_buffers.erase(ubo_handle);
}

void GraphicalProgram::addShaderInput(const ShaderInput& desc)
{
	m_shaderInputs.push_back( desc );
}

void GraphicalProgram::addUniformVariable(const UniformVarInput& def)
{
	m_uniformVars.push_back( def );
}

GraphicalProgram::UniformVarInput::UniformVarInput(const char* name,Types type,u8 rows,u8 columns,u64 dataSize,const void* data):
m_type(type),m_nbColumns(columns),m_nbRows(rows)
{
	NEP_ASSERT( name != nullptr && data != nullptr );
	
	char* u_name = new char[strlen(name) + 1];
	strcpy(u_name,name);
	m_name = u_name;

	void* u_data = new char[dataSize];
	m_data = u_data;
}

void GraphicalProgram::UniformVarInput::destruct()
{
	delete[] m_name;
	delete[] m_data;
}

