#ifndef NEPTUNE_GRAPHICS_PROGRAM_H_
#define NEPTUNE_GRAPHICS_PROGRAM_H_

#include "System/Type/Integers.h"
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

		void add(u32 shader);
		bool build();
		u32  getId()		  { return m_program; }

		// You must prefix each variable name by "block_name."
		// Returns the uniform buffer object handle of the newly created buffer
		u32  setUniformBlock(const char* block_name, const char** variables_name, const UniformBlockData* values, const u32 size);
		void deleteUniformBlock(const u32 ubo_handle);

		void setUniformVariable(const char* name, const UniformBlockData& value);
		void listActiveUniformVariables() const;

	private:
		u32 m_program;
		std::map<u32, u8*> m_uniform_block_buffers;
	};
}

#endif