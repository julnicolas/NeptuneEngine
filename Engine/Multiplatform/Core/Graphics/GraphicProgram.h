#ifndef NEPTUNE_GRAPHICS_PROGRAM_H_
#define NEPTUNE_GRAPHICS_PROGRAM_H_

#include <GL/glew.h>
#include <map>

namespace Neptune
{
	class GraphicProgram
	{
	public:
		struct UniformBlockData
		{
			size_t	m_size;
			void*	m_data;
		};

		GraphicProgram();
		~GraphicProgram();

		void add(GLuint shader);
		bool build();
		GLuint getId()				{return m_program;}

		// You must prefix each variable name by "block_name."
		// Returns the uniform buffer object handle of the newly created buffer
		GLuint setUniformBlock(const GLchar* block_name, const GLchar** variables_name, const UniformBlockData* values, const GLuint size);
		void deleteUniformBlock(const GLuint ubo_handle);

		void setUniformVariable(const GLchar* name, const UniformBlockData& value);
		void listActiveUniformVariables() const;

	private:
		GLuint m_program;
		std::map<GLuint, GLubyte*> m_uniform_block_buffers;
	};
}

#endif