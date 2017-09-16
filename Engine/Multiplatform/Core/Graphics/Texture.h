#pragma once

#include "StdInterface/Updatable.h"
#include "System/Type/Integers.h"


// Supported texture formats : ktx
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
			RGBA
		};

		struct MetaData
		{
			union HeightOr1DArrayItemCount
			{
				u32 m_height;
				u32 m_1DItemCount;
			};

			union DepthOr2DArrayItemCount
			{
				u32 m_depth;
				u32 m_2DItemCount;
			};

			u32				m_width;
			u32				m_height;																/// Also number of elements for a 1D array
			u32				m_depth;																/// Also number of elements for a 2D array
			u32				m_size;																	/// Size of the texture data
			u8				m_mipmapLevels;															/// Number of mipmap levels (>0)
			Type			m_type;
			InternalFormat	m_internalFormat;
		};

		/// 
		/// \brief Default constructor. Defined for the class to be usable by STL containers.
		/// \note Default texture type is TEXTURE_2D.
		///
		Texture();

		/// Loads a texture from a file
		Texture(const char* _path, Type _type = Type::TEXTURE_2D);

		~Texture();
		Texture(const Texture& t)				= delete;											/// Copy or move is not enabled because copying the data on the GPU side boils down to creating a new texture.
		Texture(Texture&&)						= delete;											/// Copy or move is not enabled because copying the data on the GPU side boils down to creating a new texture.
		Texture& operator=(const Texture& t)	= delete;											/// Copy or move is not enabled because copying the data on the GPU side boils down to creating a new texture.
		Texture& operator=(Texture&&)			= delete;											/// Copy or move is not enabled because copying the data on the GPU side boils down to creating a new texture.

		bool init()      final override;
		bool update()    final override;
		void terminate() final override;

		void setPath(const char* _path);
		void setIndex(u32 _index);																	/// Set texture index for use in shaders
		void setData(void* _data, u32 _size);
		void setType(Type _type)					{	m_metaData.m_type = _type;				}

		u32         getWidth()				const	{	return m_metaData.m_width;				}
		u32         getHeight()				const	{	return m_metaData.m_height;				}
		u32			getDepth()				const	{	return m_metaData.m_depth;				}
		const char* getName()				const	{	return m_path;							}
		u32			getMaxTextureCount()	const;													/// Returns the number of textures supported
		u32			getIndex()				const	{	return m_index;							}	/// Get texture's shader index

	private:
		u32				m_name;
		u32				m_index;																	/// Texture index in shaders
		MetaData		m_metaData;
		char*			m_path;
	};
}