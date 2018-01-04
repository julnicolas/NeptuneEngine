#include "GraphicsProgram.h"
#include "Debug/NeptuneDebug.h"
#include "Graphics/IncludeOpenGL.h"
#include "Graphics/Texture.h"
#include "System/Hashing/FastHashFunctions.h"
#include <string>

using namespace Neptune;

//////////////////////////////////////////////////////////////////////////////////////
//
//								S T A T I C S
//
//////////////////////////////////////////////////////////////////////////////////////


static u32 s_default_pgm_name_count = 0;
static void GenerateUniqueDefaultPgmName(GraphicsProgram::ProgramName* _pgmName, char** _stringName)
{
	// Generate a unique program name 
	NEP_ASSERT(s_default_pgm_name_count < (u32) ~0); // too many programs...
	s_default_pgm_name_count++;
	const char NAME_ROOT[] = "DEFAULT_PROGRAM_NAME_";

	std::string default_name = NAME_ROOT;
	default_name            += std::to_string(s_default_pgm_name_count);

	// Call hash function
	size_t length = default_name.length();
	*_pgmName     = Fnv1a32(reinterpret_cast<const u8*>(default_name.c_str()), length);

	// Create the debug name if necessary
#ifndef NEP_FINAL
	NEP_ASSERT(_stringName != nullptr);
	*_stringName = new char[length+1];
	strcpy_s(*_stringName, length+1, default_name.c_str());
#endif
}

static void CommonCreate(u32& _pgmID)
{
	_pgmID = glCreateProgram();
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//					C L A S S   I M P L E M E N T A T I O N
//
////////////////////////////////////////////////////////////////////////////////////////


GraphicsProgram::GraphicsProgram():
	m_programId(0)
{
	CommonCreate(m_programId);

#ifndef NEP_FINAL
		GenerateUniqueDefaultPgmName(&m_programName, &m_debugName);
#else
		GenerateUniqueDefaultPgmName(&m_programName, nullptr);
#endif
}

GraphicsProgram::GraphicsProgram(const char* _programName):
	m_programId(0)
{
	NEP_ASSERT(_programName != nullptr);

	CommonCreate(m_programId);

	// Set the program name
	size_t length   = strlen(_programName);
	m_programName   = Fnv1a32(reinterpret_cast<const u8*>(_programName), length);
#ifndef NEP_FINAL
	m_debugName = new char[length+1];
	strcpy_s(m_debugName, length+1, _programName);
#endif
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

#ifndef NEP_FINAL
	delete[] m_debugName;
#endif
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

void GraphicsProgram::setTexture(Texture* _texture)
{
	NEP_ASSERT(_texture != nullptr); // Error: _texture pointer is invalid
	m_textures[_texture->getIndex()] = _texture;
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
	NEP_ASSERT( name != nullptr && data != nullptr );
	
	size_t name_length = strlen(name)+1;
	char* u_name = new char[name_length];
	strcpy_s(u_name, name_length, name);
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

void GraphicsProgram::UniformVarInput::setData(const void* data)
{
	NEP_ASSERT(data != nullptr); // Error, empty data.
	memcpy( m_data, data, m_nbColumns*m_nbRows*GraphicsProgram::getTypeSize(m_type) );
}

void GraphicsProgram::UniformVarInput::destruct()
{
	delete[] m_name;
	delete[] m_data;
}

