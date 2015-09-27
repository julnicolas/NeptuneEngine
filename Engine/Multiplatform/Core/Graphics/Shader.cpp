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
	int length = file.tellg();
	file.seekg(0, file.beg);

	GLchar* code = new GLchar[length];
	std::memset(code, '\0', length);	// We won't get any unwanted caracter
	
	file.read( code, length );

	// Makes the shader usable

	m_shader = glCreateShader(shader_type);
	glShaderSource(m_shader, 1, &code, NULL);
	glCompileShader(m_shader);

	GLint comp_status;
	glGetShaderiv(m_shader, GL_COMPILE_STATUS, &comp_status);
	if ( comp_status != GL_TRUE )
	{
		NEP_LOG("Error. Shader hasn't been compiled successfully.");
		NEP_LOG(code);
		NEP_ASSERT(false);
		return;
	}

	// Cleaning out

	file.close();
	delete[] code;
}

Neptune::Shader::~Shader()
{
	glDeleteShader(m_shader);
}