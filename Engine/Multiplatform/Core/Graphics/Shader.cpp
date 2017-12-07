#include "Shader.h"
#include <fstream>
#include <sstream>
#include "Debug/NeptuneDebug.h"

Neptune::Shader::Shader(const GLchar* file_name, GLuint shader_type):
	m_shader(0)
{
	// Load shader code
	
	std::ifstream file(file_name);
	if ( !file.is_open() )
	{
		NEP_LOG("Error. Shader file couldn't be found.");
		NEP_ASSERT(false);
		return;
	}

	file.seekg(0, file.end);
	auto length = file.tellg();
	file.seekg(0, file.beg);

	GLchar* code = new GLchar[length];
	std::memset(code, '\0', length);	// We won't get any unwanted character
	
	file.read( code, length );

	// Makes the shader usable

	m_shader = glCreateShader(shader_type);
	glShaderSource(m_shader, 1, &code, NULL);
	glCompileShader(m_shader);

	GLint comp_status;
	glGetShaderiv(m_shader, GL_COMPILE_STATUS, &comp_status);
	if ( comp_status != GL_TRUE )
	{
		NEP_LOG("Error. Shader hasn't been compiled successfully.\nShader's name: %s\nCode:\n%s", file_name, code);
		
		// Get error message's size
		int buffer_size = 0;
		glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &buffer_size);

		// Get error message
		char* buffer = new char[buffer_size];
		int returned_buffer_length = 0;
		glGetShaderInfoLog(m_shader, buffer_size, &returned_buffer_length, buffer);

		// Print message
		NEP_ASSERT_ERR_MSG(buffer_size == returned_buffer_length+1, "Log buffer mismatch - Could glGetShaderiv access the log's message length?");
		NEP_LOG("Error message : %s ", buffer);
		delete[] buffer;
		NEP_ASSERT(false);
	}

	// Cleaning out

	file.close();
	delete[] code;
}

Neptune::Shader::~Shader()
{
	glDeleteShader(m_shader);
}