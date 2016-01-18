#include "GraphicsProgram.h"
#include "Debug/NeptuneDebug.h"
#include "Graphics/IncludeOpenGL.h"
#include "System/Hashing/FastHashFunctions.h"

using namespace Neptune;

GraphicsProgram::GraphicsProgram():
	m_programId(0)
{
	m_programId = glCreateProgram();
}

GraphicsProgram::~GraphicsProgram()
{
	// Destroy its uniform variables
	{
		UniformVarIterator it_end = m_uniformVars.end();
		for(UniformVarIterator it = m_uniformVars.begin(); it != it_end; ++it)
			it->second.destruct();
	}
	
	// Destroy its uniform blocks
	{
		std::map<u32, u8*>::iterator it_end = m_uniformBlockBuffers.end();
		for (std::map<u32, u8*>::iterator it = m_uniformBlockBuffers.begin(); it != it_end; ++it)
			delete[] it->second;
	}

	// Delete the program from VRAM
	glDeleteProgram( m_programId );
}

void GraphicsProgram::add(u32 shader)
{
	glAttachShader(m_programId, shader);
}

bool GraphicsProgram::build()
{
	glLinkProgram(m_programId);

	return true;
}

void GraphicsProgram::addUniformBlock(const char* blockName, const char** variablesName, const UniformBlockData* values, const u32 size)
{
    // Get the index of the uniform block
    u32 block_index = glGetUniformBlockIndex(m_programId, blockName);

    // Allocate space for the buffer
    s32 block_size;
    glGetActiveUniformBlockiv(m_programId, block_index,
                              GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);
	
	u8* block_buffer = new u8[block_size];

    // Query the offsets of each block variable
    u32* indices = new u32[size];
    glGetUniformIndices(m_programId, size, variablesName, indices);
    
    s32* offset = new s32[size];
    glGetActiveUniformsiv(m_programId, size, indices, GL_UNIFORM_OFFSET, offset);

    // Store data within the buffer at the appropriate offsets
    for (u32 i = 0; i < size; i++)
		memcpy(block_buffer + offset[i], values[i].m_data, values[i].m_size);
	

    // Create the buffer object and copy the data
    u32 ubo_handle;
    glGenBuffers( 1, &ubo_handle );
    glBindBuffer( GL_UNIFORM_BUFFER, ubo_handle );			// Bind to modify the buffer
    glBufferData( GL_UNIFORM_BUFFER, block_size, block_buffer, GL_DYNAMIC_DRAW );

	// Compute the value of the binding point
	u32 binding_point = m_uniformBlockBuffers.size();

    // Bind the buffer object to the uniform block
    glBindBufferBase( GL_UNIFORM_BUFFER, binding_point, ubo_handle );	// Bound to be used as a source parameter in the shader
    
    // We don't need this if we specify the binding within the shader
	// glUniformBlockBinding(programHandle, block_index, 0);

	// Cleaning out 

	delete[] indices;
	delete[] offset;

	// Add the new buffer to the program list of uniform buffers

	m_uniformBlockBuffers[ubo_handle] = block_buffer;
}

void GraphicsProgram::rmUniformBlock(const u32 ubo_handle)
{
	delete[] m_uniformBlockBuffers[ubo_handle];
	m_uniformBlockBuffers.erase(ubo_handle);
}

void GraphicsProgram::addShaderAttribute(const ShaderAttribute& desc)
{
	m_shaderAttributes.push_back( desc );
}

void GraphicsProgram::addUniformVariable(const UniformVarInput& def)
{
	// Foolish but it's a pain in the ass to code a new hash function for std::unordered_map
	// it's late I want to go to bed
	m_uniformVars.insert( { (const char*) Fnv1a32( (u8*) def.getName(), strlen(def.getName()) ), def} );
}

GraphicsProgram::UniformVarIterator GraphicsProgram::getUniformVar(const char* name)
{
	// Same applies here (the pain in da butt)
	return m_uniformVars.find( (const char*) Fnv1a32( (u8*) name, strlen(name) ) );
}

GraphicsProgram::UniformVarInput::UniformVarInput(const char* name,Types type,u8 rows,u8 columns,u64 dataSize,const void* data):
m_type(type),m_nbColumns(columns),m_nbRows(rows)
{
	NEP_ASSERT( m_nbRows > 0 && m_nbRows <= 4 );
	NEP_ASSERT( m_nbColumns > 0 && m_nbColumns <= 4 );
	NEP_ASSERT( name != nullptr && data != nullptr );
	
	char* u_name = new char[strlen(name) + 1];
	strcpy(u_name,name);
	m_name = u_name;

	void* u_data = new char[dataSize];
	memcpy( u_data, data, dataSize );
	m_data = u_data;
}

size_t GraphicsProgram::getTypeSize(Types t)
{
	switch (t)
	{
	case FLOAT:
		return sizeof(float);

	case U8:
		return sizeof(u8);

	case U32:
		return sizeof(u32);

	case S32:
		return sizeof(s32);

	default:
		NEP_ASSERT(false);
		return ~0;
	}
}

void GraphicsProgram::UniformVarInput::setData(void* data)
{
	memcpy( m_data, data, m_nbColumns*m_nbRows*GraphicsProgram::getTypeSize(m_type) );
}

void GraphicsProgram::UniformVarInput::destruct()
{
	delete[] m_name;
	delete[] m_data;
}

