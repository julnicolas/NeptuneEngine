#pragma once

#include "System/Type/Integers.h"
#include <map>
#include <vector>

namespace Neptune
{
	///
	/// Represents a full OpenGL program (with all its shaders).
	///

	class GraphicalProgram
	{
	public:

		///
		/// Useful to determine which OpenGL function to use to pass an uniform.
		///
		enum Types : u8
		{
			U8,
			U16,
			U32,
			U64,
			S8,
			S16,
			S32,
			S64,
			FLOAT,
			DOUBLE
		};

		///
		/// Structure used to describe a vertex shader input variable.
		/// Because the data-size can be pretty big, the values will be directly passed to a renderer.
		///
		struct ShaderInput
		{
			u8    m_layout;         /// Variable's layout within the vertex shader.
			Types m_type;           /// Type of the values.
			u8    m_nbComponents;   /// Number of components of each value.
		};

		///
		/// Represents an uniform variable
		///
		class UniformVarInput
		{
		public:
			friend class GraphicalProgram;

			///
			/// COPY the input parameters to initialize the object.
			///
			UniformVarInput(const char* name,Types type,u8 rows,u8 columns,u64 dataSize,const void* data);
			
			const char* getName()      const { return m_name;      }
			Types       getDataType()  const { return m_type;      }
			u8          getNbRows()    const { return m_nbRows;    }
			u8          getNbColumns() const { return m_nbColumns; }
			const void* getData()      const { return m_data;      }

			~UniformVarInput()                                 = default; /// Destruction of dynamically allocated memory is managed by the enclosing class.
			UniformVarInput(const UniformVarInput&)            = default;
			UniformVarInput& operator=(const UniformVarInput&) = default;

		private:
			void destruct();            /// Used by the enclosing class when it's about to get destructed.

			const char* m_name;         /// Name of the uniform variable.
			Types       m_type;         /// Type of the variable
			u8          m_nbRows;       /// Must be between 1 and 4. If greater than 1 then the variable is a vector
			u8          m_nbColumns;    /// Must be between 1 and 4 and can only be if m_nbRows > 1. If greater than 1 (and of course m_nbRows > 1) then the variable is matrix. 
			void*       m_data;         /// The uniform's value.
		};

		///
		/// Represents a uniform block's inner-variable.
		///
		struct UniformBlockData
		{
			size_t	m_size; /// Size of the data (ex: sizeof(float))
			void*	m_data; /// Its value (ex: float x=0.0f; m_data = &x;)
		};

		GraphicalProgram();
		~GraphicalProgram();

		void add(u32 shader);
		bool build();
		u32  getId() const		  { return m_program; }

		void addShaderInput(const ShaderInput& desc); /// Add a vertex-shader-input-description.
		void addUniformVariable(const UniformVarInput& def);

		///
		/// Creates an uniform block variable.
		/// The content of values is directly copied into VRAM. Therefore, the values can be volatile data.
		/// \Param blockName The name of the uniform block within the vertex shader (ex: BlobSettings).
		/// \Param variablesName Names of the inner variables of the uniform blocks. Must always be prefixed
		/// by blockName (ex: BlobSettings.Color)
		/// \Param values Array of values with each block corresponding to an inner variable.
		/// \Param nbValues Number of UniformBlockData held by values.
		/// \Return The id of the newly created uniform block buffer.
		///
		u32  addUniformBlock(const char* blockName,const char** variablesName,const UniformBlockData* values,const u32 nbValues);
		void rmUniformBlock(const u32 ubo_handle); /// Delete an uniform block from VRAM.

	private:
		u32 m_program;
		std::vector<UniformVarInput> m_uniformVars; /// Contains every vertex-shader's uniform variables.
		std::vector<ShaderInput> m_shaderInputs;    /// Contains every vertex-shader description.
		std::map<u32, u8*> m_uniform_block_buffers; //! Must certainly be refactored
	};
}
