#ifndef NEPTUNE_SHADER_H_
#define NEPTUNE_SHADER_H_

#include <GL/glew.h>

namespace Neptune
{
	class Shader
	{
	public:
		Shader(const GLchar* file_name, GLuint shader_type);
		~Shader();

		GLuint getId()	{return m_shader;}
	
		// The = operator and the copy constructor will certainly have to be defined
	
	private:
		GLuint		m_shader;
	};
}

#endif