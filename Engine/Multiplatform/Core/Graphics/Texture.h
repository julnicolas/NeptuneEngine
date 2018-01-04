#pragma once

#include "StdInterface/Updatable.h"
#include "System/Type/Integers.h"
#include <string>


namespace Neptune
{
	/// \class		Class that instanciate a texture for use in shaders.
	///				RAM is managed by RAII (constructors and destructors)
	///				VRAM is managed through the use of init, update and terminate (Updatable interface).
	///				Finally, the current supported formats are:
	///				- texture formats : ktx
	///				- image formats: "png", "jpg", "jpeg", "tga", "bmp", "psd", "gif", "hdr", "pic"
	///
	///	\warning	Calling terminate() is absolutely critical. Otherwise VRAM will be leaked.
	class Texture final: public Updatable
	{
	public:
		enum class Type : u8
		{
			TEXTURE_1D,
			TEXTURE_2D,
			TEXTURE_3D,
			CUBE_MAP,
			BUFFER,				/// Texture of really large size. NOT SUPPORTED YET.
			TEXTURE_ARRAY_1D,
			TEXTURE_ARRAY_2D,
			CUBE_MAP_ARRAY,
			NOT_SUPPORTED
		};

		enum class InternalFormat : u8
		{
			RED,
			RG,
			RGB,
			RGBA,
			NOT_SUPPORTED
		};

		struct MetaData
		{
			u32				m_width;
			u32				m_height;																/// Also number of elements for a 1D array
			u32				m_depth;																/// Also number of elements for a 2D array
			u32				m_size;																	/// Size of the texture data
			u8				m_mipmapLevels;															/// Number of mipmap levels (>0)
			Type			m_type;
			InternalFormat	m_internalFormat;														/// \warning Cannot always be determined
		};

		/// 
		/// \brief Default constructor. Defined for the class to be usable by STL containers.
		/// \note Default texture type is TEXTURE_2D.
		///
		Texture();

		/// Loads a texture from a file
		Texture(const char* _path, Type _type = Type::TEXTURE_2D);

		Texture(Texture&&);																			/// Transfer full texture ownership to the new entity
		Texture& operator=(Texture&&);																/// Transfer full texture ownership to the new entity

		~Texture();

		Texture(const Texture& t)				= delete;											/// Copy is not enabled because copying the data on the GPU side boils down to creating a new texture.
		Texture& operator=(const Texture& t)	= delete;											/// Copy is not enabled because copying the data on the GPU side boils down to creating a new texture.

		bool init()      final override;
		bool update()    final override;
		void terminate() final override;															/// \warning Must be called absolutely, otherwise VRAM will be leaked. This method deallocates VRAM.

		void setPath(const char* _path);
		void setIndex(u32 _index);																	/// Set texture index for use in shaders
		void setData(void* _data, u32 _size);
		void setType(Type _type)					{	m_metaData.m_type = _type;				}

		u32         getWidth()				const	{	return m_metaData.m_width;				}
		u32         getHeight()				const	{	return m_metaData.m_height;				}
		u32			getDepth()				const	{	return m_metaData.m_depth;				}
		const char* getPath()				const	{	return m_path;							}
		u32			getMaxTextureCount()	const;													/// Returns the number of textures supported
		u32			getIndex()				const	{	return m_index;							}	/// Get texture's shader index

		static std::string GetStandardDir()		{	return "Resources/Textures/";			}	/// Standard path where textures should be stored. This path can be used as a reference by higher level tools.

	private:
		u32					m_name;
		u32					m_index;																	/// Texture index in shaders
		MetaData			m_metaData;
		char*				m_path;
	};
}