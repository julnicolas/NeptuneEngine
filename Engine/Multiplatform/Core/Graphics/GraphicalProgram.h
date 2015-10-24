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

		//////////////////////////////////////////////////////////////////////////////
		//
		//								E N U M S
		//
		/////////////////////////////////////////////////////////////////////////////


		///
		/// Useful to determine which OpenGL function to use to pass an uniform.
		///
		enum Types : u8
		{
			U32,
			S32,
			FLOAT
		};


		//////////////////////////////////////////////////////////////////////////////
		//
		//			I N N E R   C L A S S E S   A N D  S T R U C T S 
		//
		/////////////////////////////////////////////////////////////////////////////


		///
		/// Structure used to describe a vertex shader input variable.
		/// Because the data-size can be pretty big, the values will be directly passed to a renderer.
		///
		struct ShaderAttribute
		{
			u8     m_layout;         /// Variable's layout within the vertex shader.
			Types  m_type;           /// Type of the values.
			u8     m_nbComponents;   /// Number of components of each value.
			size_t m_size;           /// Data size
			void*  m_data;           /// Data to be copied into VRAM. It is copied in the init call.
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


		//////////////////////////////////////////////////////////////////////////
		//
		//					T Y P E   D E F I N I T I O N S
		//
		/////////////////////////////////////////////////////////////////////////


		typedef std::vector<UniformVarInput>::iterator       UniformVarIterator;
		typedef std::vector<UniformVarInput>::const_iterator ConstUniformVarIterator;
		typedef std::vector<ShaderAttribute>::iterator       ShaderAttributeIterator;
		typedef std::vector<ShaderAttribute>::const_iterator ConstShaderAttributeIterator;


		/////////////////////////////////////////////////////////////////////////
		//
		//							M E T H O D S
		//
		////////////////////////////////////////////////////////////////////////


		GraphicalProgram();
		~GraphicalProgram();

		void add(u32 shader);
		bool build();
		u32  getId() const	{ return m_programId; }

		void addShaderAttribute(const ShaderAttribute& desc); /// Adds a vertex-shader-input-description.
		u8                           getNbVertexAttributes() const  { return (u8) m_shaderAttributes.size(); }
		ShaderAttributeIterator      shaderAttributeBegin()         { return m_shaderAttributes.begin();     }
		ShaderAttributeIterator      shaderAttributeEnd()           { return m_shaderAttributes.end();       }
		ConstShaderAttributeIterator shaderAttributeCBegin() const  { return m_shaderAttributes.cbegin();    }
		ConstShaderAttributeIterator shaderAttributeCEnd()   const  { return m_shaderAttributes.cend();      }

		void addUniformVariable(const UniformVarInput& def);
		UniformVarIterator      uniformVarBegin()          { return m_uniformVars.begin();    }
		UniformVarIterator      uniformVarEnd()            { return m_uniformVars.end();      }
		ConstUniformVarIterator uniformVarCBegin() const   { return m_uniformVars.cbegin();   }
		ConstUniformVarIterator uniformVarCEnd()   const   { return m_uniformVars.cend();     }

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
		void addUniformBlock(const char* blockName,const char** variablesName,const UniformBlockData* values,const u32 nbValues);
		void rmUniformBlock(const u32 ubo_handle); /// Delete an uniform block from VRAM.

	private:
		u32 m_programId;
		std::vector<UniformVarInput> m_uniformVars;       /// Contains every vertex-shader's uniform variables.
		std::vector<ShaderAttribute> m_shaderAttributes;  /// Contains every vertex-shader-attribute description.

		// Bad design
		std::map<u32, u8*> m_uniformBlockBuffers;  /// Must be refactored

	};
}
