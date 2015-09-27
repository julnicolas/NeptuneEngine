#include "GraphicProgram.h"
#include <GL/glew.h>
#include <cstring>

using namespace Neptune;

GraphicProgram::GraphicProgram():
	m_program(0)
{
	m_program = glCreateProgram();
}

GraphicProgram::~GraphicProgram()
{
	std::map<GLuint, GLubyte*>::iterator it_end = m_uniform_block_buffers.end();
	for (std::map<GLuint, GLubyte*>::iterator it = m_uniform_block_buffers.begin(); it != it_end; ++it)
		delete[] it->second;
}

void GraphicProgram::add(GLuint shader)
{
	glAttachShader(m_program, shader);
}

bool GraphicProgram::build()
{
	glLinkProgram(m_program);
	
	return true;
}

GLuint GraphicProgram::setUniformBlock(const GLchar* block_name, const GLchar** variables_name, const UniformBlockData* values, const GLuint size)
{
	
    // Get the index of the uniform block
    GLuint blockIndex = glGetUniformBlockIndex(m_program, block_name);

    // Allocate space for the buffer
    GLint blockSize;
    glGetActiveUniformBlockiv(m_program, blockIndex,
                              GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	
	GLubyte* blockBuffer = new GLubyte[blockSize];

    // Query for the offsets of each block variable
    GLuint* indices = new GLuint[size];
    glGetUniformIndices(m_program, size, variables_name, indices);
    
    GLint* offset = new GLint[size];
    glGetActiveUniformsiv(m_program, size, indices, GL_UNIFORM_OFFSET, offset);

    // Store data within the buffer at the appropriate offsets
    for (GLuint i = 0; i < size; i++)
		memcpy(blockBuffer + offset[i], values[i].m_data, values[i].m_size);
	

    // Create the buffer object and copy the data
    GLuint uboHandle;
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

void GraphicProgram::deleteUniformBlock(const GLuint ubo_handle)
{
	delete[] m_uniform_block_buffers[ubo_handle];
	m_uniform_block_buffers.erase(ubo_handle);
}

void GraphicProgram::setUniformVariable(const GLchar* name, const UniformBlockData& value)
{

}

void GraphicProgram::listActiveUniformVariables() const
{

}
