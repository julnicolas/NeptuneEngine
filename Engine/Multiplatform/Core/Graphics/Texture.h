#pragma once

#include "StdInterface/Updatable.h"
#include "System/Type/Integers.h"


// SUpported texture formats : ktx
// Supported image formats: "png", "jpg", "jpeg", "tga", "bmp", "psd", "gif", "hdr", "pic"


namespace Neptune
{
	class Texture final: public Updatable
	{
	public:
		enum class Type : u8
		{
			TEXTURE_1D,
			TEXTURE_2D,
			TEXTURE_3D,
			CUBE_MAP,
			BUFFER,				/// Texture of really large size
			MULTI_SAMPLE_2D,	/// Used for anti-aliasing filters
			TEXTURE_ARRAY_1D,
			TEXTURE_ARRAY_2D,
			TEXTURE_ARRAY_2D_MULTISAMPLE,
			CUBE_MAP_ARRAY,
		};

		/// 
		/// \brief Default constructor. Defined for the class to be usable by STL containers.
		/// \note Default texture type is TEXTURE_2D.
		///
		Texture();

		/// Loads a texture from a file
		Texture(const char* _path, Type _type = Type::TEXTURE_2D);
		
		/// 
		/// \brief Loads a Texture from a buffer. The texture discard the buffer after init() is called.
		/// \brief The content is then stored in VRAM.
		///
		Texture(void* _data, u32 _size, Type _type = Type::TEXTURE_2D);

		~Texture();
		Texture(const Texture& t)				= delete;
		Texture(Texture&&)						= delete;
		Texture& operator=(const Texture& t)	= delete;
		Texture& operator=(Texture&&)			= delete;

		bool init()      final override;
		bool update()    final override;
		void terminate() final override;

		void setPath(const char* _path);
		void setData(void* _data, u32 _size);
		void setType(Type _type)				{	m_type = _type;					}

		u32         getWidth()          const	{	return m_width;					}
		u32         getHeight()         const	{	return m_height;				}
		u32			getDepth()			const	{	return m_depth;					}
		u32         getNbCompPerPixel() const	{	return m_componentsPerPixel;	}
		const char* getName()           const	{	return m_path;					}
		bool        isInitialized()     const	{	return m_textureID != 0;		}

	private:
		void CreateTexture(u8* data, s32 reqComp);
		void SetPlaceHolderTexture(u8*& data);

		u32				m_textureID;
		u32				m_width;
		u32				m_height;
		u32				m_depth;																	/// Used for 3D textures
		u32				m_componentsPerPixel;
		Type			m_type;
		u32				m_size;																		/// Size of the texture data
		void*			m_data;																		/// Used when setting texture data at runtime rather than from a file
		char*			m_path;
	};
}