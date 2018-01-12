#pragma once

#include "System/Type/Integers.h"
#include "Maintenance/CompilerHints.h"
#include <map>
#include <unordered_map>

namespace Neptune
{
	class Texture;

	///
	/// Represents a full OpenGL program (with all its shaders).
	///

	class GraphicsProgram
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
			U8,
			U32,
			S32,
			FLOAT
		};
		static size_t getTypeSize(Types);


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
			bool   m_normalized;     /// Should data be normalized?
			size_t m_size;           /// Data size
			void*  m_data;           /// Data to be copied into VRAM. It is copied in the init call.
		};

		///
		/// Represents an uniform variable
		///
		class UniformVarInput
		{
		public:
			friend class GraphicsProgram;

			///
			/// COPY the input parameters to initialize the object.
			/// \Param name The name of the uniform variable in the shader program.
			/// \Param type The value's type.
			/// \Param rows The number of rows that makes up the value. Vectors and matrices are represented in the row-major order.
			/// \Param columns The number of columns that makes up the value. If nb rows > 1 and nb cols > 1 then the value is a matrix.
			/// \Param dataSize The size of the value in bytes.
			/// \Param A pointer to the value.
			/// \Warning Data parameter: Adjacent data-blocks are expected. Indeed, the parameter's content will be copied into an array.
			///
			UniformVarInput(const char* name,Types type,u8 rows,u8 columns,u64 dataSize,const void* data);

			///
			/// \warning This constructor is here for the sole purpose of this class being usable by std containers.
			/// Use the other constructor instead.
			///
			UniformVarInput() = default;

			const char* getName()      const { return m_name;      }
			Types       getDataType()  const { return m_type;      }
			u8          getNbRows()    const { return m_nbRows;    }
			u8          getNbColumns() const { return m_nbColumns; }
			const void* getData()      const { return m_data;      }
			
			NEP_DEPRECATED("unsafe, data size should be controlled")
				void setData(const void* _data);

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


		typedef std::unordered_map<const char*, UniformVarInput>::iterator			UniformVarIterator;
		typedef std::unordered_map<const char*, UniformVarInput>::const_iterator	ConstUniformVarIterator;
		typedef std::vector<ShaderAttribute>::iterator								ShaderAttributeIterator;
		typedef std::vector<ShaderAttribute>::const_iterator						ConstShaderAttributeIterator;
		typedef std::unordered_map<u32, Texture*>::iterator							TextureIterator;
		typedef std::unordered_map<u32, Texture*>::const_iterator					ConstTextureIterator;
		typedef u32																	ProgramName;


		/////////////////////////////////////////////////////////////////////////
		//
		//							M E T H O D S
		//
		////////////////////////////////////////////////////////////////////////


		GraphicsProgram();												/// \note A unique default name is generated for the program
		GraphicsProgram(const char* _programName);						/// \note Sets a user-defined name for the program
		~GraphicsProgram();
		GraphicsProgram(const GraphicsProgram&)				= delete;
		GraphicsProgram(GraphicsProgram&&)					= delete;
		GraphicsProgram&  operator=(const GraphicsProgram&)	= delete;
		GraphicsProgram&  operator=(GraphicsProgram&&)		= delete;

		ProgramName getName() const									{ return m_programName;					}	/// Returns the user-defined name. Useful for high level classes.

#ifndef NEP_FINAL
		const char* getDebugName() const							{ return m_debugName;			 }
#endif
		void add(u32 shader); // TODO: take a const Shader& instead of a u32
		bool build();
		u32  getResourceID() const	{ return m_programId; }														/// Returns the resource ID given by the graphics library. Useful for low-level rendering-related classes.

		void addShaderAttribute(const ShaderAttribute& desc);													/// Adds a vertex-shader-input-description.
		u8                           getNbVertexAttributes() const  { return (u8) m_shaderAttributes.size(); }
		ShaderAttributeIterator      shaderAttributeBegin()         { return m_shaderAttributes.begin();     }
		ShaderAttributeIterator      shaderAttributeEnd()           { return m_shaderAttributes.end();       }
		ConstShaderAttributeIterator shaderAttributeCBegin() const  { return m_shaderAttributes.cbegin();    }
		ConstShaderAttributeIterator shaderAttributeCEnd()   const  { return m_shaderAttributes.cend();      }

		void addUniformVariable(const UniformVarInput& def);
		UniformVarIterator      getUniformVar(const char* name);
		UniformVarIterator      uniformVarBegin()					{ return m_uniformVars.begin();			}
		UniformVarIterator      uniformVarEnd()						{ return m_uniformVars.end();			}
		ConstUniformVarIterator uniformVarCBegin() const			{ return m_uniformVars.cbegin();		}
		ConstUniformVarIterator uniformVarCEnd()   const			{ return m_uniformVars.cend();			}

		/// \brief Adds a texture to the program. 
		/// If _tex has the index then it will replace the one currently stored by the program.
		/// \warning Pay attention if you share the program (like in ViewSpawner), because all objects using
		/// this program will see their texture swapped by _tex. This may not be the effect you would like.
		void setTexture(Texture* _tex);																				 
		TextureIterator      textureBegin()							{ return m_textures.begin();			}
		TextureIterator      textureEnd()							{ return m_textures.end();				}
		ConstTextureIterator textureCBegin() const					{ return m_textures.cbegin();			}
		ConstTextureIterator textureCEnd()   const					{ return m_textures.cend();				}

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
		u32													m_programId;			/// Resource ID given by the graphics library
		u32													m_programName;			/// User-defined program name
		std::unordered_map<const char*, UniformVarInput>	m_uniformVars;			/// Contains every vertex shader's uniform variables.
		std::vector<ShaderAttribute>						m_shaderAttributes;		/// Contains every vertex-shader-attribute description.
		std::unordered_map<u32,Texture*>					m_textures;				/// Program's texture set. The key is the texture's ID.

		// Bad design
		std::map<u32, u8*> m_uniformBlockBuffers;  /// Must be refactored

#ifndef NEP_FINAL
		///
		/// Non-hashed program name for debug purpose only.
		/// \Attention For debug use only. Could cause crashes if not used in the right configuration.
		///
		char* m_debugName;

#endif

	};
}
