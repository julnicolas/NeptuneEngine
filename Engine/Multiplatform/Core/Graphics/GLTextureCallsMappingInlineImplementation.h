#pragma once

#include "Graphics/GLTextureCallsMapping.h"
#include "Debug/NeptuneDebug.h"

inline Neptune::u32 Neptune::GLTextureCallsMapping::MapTextureType(Neptune::Texture::Type _type)
{
	u16 type = 0;
	switch (_type)
	{
		// S I N G L E   T E X T U R E S
	case Texture::Type::TEXTURE_1D:
		type = GL_TEXTURE_1D;
		break;

	case Texture::Type::TEXTURE_2D:
		type = GL_TEXTURE_2D;
		break;

	case Texture::Type::TEXTURE_3D:
		type = GL_TEXTURE_3D;
		break;

	case Texture::Type::CUBE_MAP:
		type = GL_TEXTURE_CUBE_MAP;
		break;

	case Texture::Type::BUFFER:
		type = GL_TEXTURE_BUFFER;
		break;

		// A R R A Y S
	case Texture::Type::TEXTURE_ARRAY_1D:
		type = GL_TEXTURE_1D_ARRAY;
		break;

	case Texture::Type::TEXTURE_ARRAY_2D:
		type = GL_TEXTURE_2D_ARRAY;
		break;

	case Texture::Type::CUBE_MAP_ARRAY:
		type = GL_TEXTURE_CUBE_MAP_ARRAY;
		break;
	}

	return type;
}

inline Neptune::Texture::Type Neptune::GLTextureCallsMapping::MapTextureType(Neptune::u32 _glType)
{
	Texture::Type type = Texture::Type::TEXTURE_2D;
	switch (_glType)
	{
		// S I N G L E   T E X T U R E S
	case GL_TEXTURE_1D:
		type = Texture::Type::TEXTURE_1D;
		break;

	case GL_TEXTURE_2D:
		type = Texture::Type::TEXTURE_2D;
		break;

	case GL_TEXTURE_3D:
		type = Texture::Type::TEXTURE_3D;
		break;

	case GL_TEXTURE_BUFFER:
		type = Texture::Type::BUFFER;
		break;

		// A R R A Y S
	case GL_TEXTURE_1D_ARRAY:
		type = Texture::Type::TEXTURE_ARRAY_1D;
		break;

	case GL_TEXTURE_2D_ARRAY:
		type = Texture::Type::TEXTURE_ARRAY_2D;
		break;

	case GL_TEXTURE_CUBE_MAP_ARRAY:
		type = Texture::Type::CUBE_MAP_ARRAY;
		break;

	default:
		type = Texture::Type::NOT_SUPPORTED;
		break;
	}

	// Test for cube map (value between GL_TEXTURE_CUBE_MAP and GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
	if (_glType >= GL_TEXTURE_CUBE_MAP && _glType <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
		type = Texture::Type::CUBE_MAP;

	return type;
}

inline GLenum Neptune::GLTextureCallsMapping::MapInternalColorFormat(Neptune::Texture::InternalFormat _internalFormat)
{
	switch (_internalFormat)
	{
	case Texture::InternalFormat::RED:
		return GL_RED;

	case Texture::InternalFormat::RG:
		return GL_RG;

	case Texture::InternalFormat::RGB:
		return GL_RGB;

	case Texture::InternalFormat::RGBA:
		return GL_RGBA;

	// Can only be hit if the code hasn't been updated or the value was passed using a reinterpret_cast
	default:
		NEP_LOG("ERROR Unsupported format in MapInternalFormat");
		NEP_ASSERT(false);
		return GL_INVALID_ENUM;
	}
}

inline GLenum Neptune::GLTextureCallsMapping::MapInternalBinaryFormat(Neptune::Texture::InternalFormat _internalFormat)
{
	switch (_internalFormat)
	{
	case Texture::InternalFormat::RED:
		return GL_R32F;

	case Texture::InternalFormat::RG:
		return GL_RG32F;

	case Texture::InternalFormat::RGB:
		return GL_RGB32F;

	case Texture::InternalFormat::RGBA:
		return GL_RGBA32F;

	// Can only be hit if the code hasn't been updated or the value was passed using a reinterpret_cast
	default:
		NEP_LOG("ERROR Unsupported format in MapInternalFormat");
		NEP_ASSERT(false);
		return GL_INVALID_ENUM;
	}
}

//! To use this function, openGL 4.3 or ARB_texture_storage must be available
inline void Neptune::GLTextureCallsMapping::GLTexStorageCubeMap(const Neptune::Texture::MetaData& _metaData)
{
	NEP_ASSERT(_metaData.m_width == _metaData.m_height); // Error the texture is not a square
	
	glTexStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height);

	NEP_GRAPHICS_ASSERT();

	glTexStorage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height);

	NEP_GRAPHICS_ASSERT();

	glTexStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height);

	NEP_GRAPHICS_ASSERT();

	glTexStorage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height);

	NEP_GRAPHICS_ASSERT();

	glTexStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height);

	NEP_GRAPHICS_ASSERT();

	glTexStorage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height);

	NEP_GRAPHICS_ASSERT();
}

inline void Neptune::GLTextureCallsMapping::GLTexStorageCubeMapArray(const Neptune::Texture::MetaData& _metaData)
{
	NEP_ASSERT(_metaData.m_width == _metaData.m_height); // Error the texture is not a square
	NEP_ASSERT(_metaData.m_depth > 0); // Warning: You probably don't want an array

	NEP_ASSERT(false); // gl enum is not correct fix it man

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);  

	NEP_GRAPHICS_ASSERT();

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);  

	NEP_GRAPHICS_ASSERT();

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);  

	NEP_GRAPHICS_ASSERT();

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);  

	NEP_GRAPHICS_ASSERT();

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);  

	NEP_GRAPHICS_ASSERT();

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);

	NEP_GRAPHICS_ASSERT();
}

inline void Neptune::GLTextureCallsMapping::GLTexSubImageCubeMap(const Neptune::Texture::MetaData& _metaData, u8 _mipmapLevel, const u8* _data)
{
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		_mipmapLevel,		// index of the current image in the mipmap list
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalColorFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data);

	NEP_GRAPHICS_ASSERT();

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		_mipmapLevel,		// index of the current image in the mipmap list
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalColorFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data);  

	NEP_GRAPHICS_ASSERT();

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		_mipmapLevel,		// index of the current image in the mipmap list
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalColorFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data);  

	NEP_GRAPHICS_ASSERT();

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		_mipmapLevel,		// index of the current image in the mipmap list
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalColorFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data);  

	NEP_GRAPHICS_ASSERT();

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		_mipmapLevel,		// index of the current image in the mipmap list
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalColorFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data);  

	NEP_GRAPHICS_ASSERT();

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		_mipmapLevel,		// index of the current image in the mipmap list
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalColorFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data);  

	NEP_GRAPHICS_ASSERT();
}

inline void Neptune::GLTextureCallsMapping::GLTexSubImageCubeMapArray(const Neptune::Texture::MetaData& _metaData, u8 _mipmapLevel, const u8** _data)
{
	NEP_ASSERT(false); // gl enum is probably not correct
	
	for (u32 i = 0; i < _metaData.m_depth; i++)
	{
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		_mipmapLevel,		// index of the current image in the mipmap list
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalColorFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data[i]);

		NEP_GRAPHICS_ASSERT();

		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		_mipmapLevel,		// index of the current image in the mipmap list
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalColorFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data[i]);  

		NEP_GRAPHICS_ASSERT();

		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		_mipmapLevel,		// index of the current image in the mipmap list
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalColorFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data[i]);  

		NEP_GRAPHICS_ASSERT();

		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		_mipmapLevel,		// index of the current image in the mipmap list
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalColorFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data[i]);  

		NEP_GRAPHICS_ASSERT();

		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		_mipmapLevel,		// index of the current image in the mipmap list
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalColorFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data[i]);  

		NEP_GRAPHICS_ASSERT();

		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		_mipmapLevel,		// index of the current image in the mipmap list
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalColorFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data[i]);

		NEP_GRAPHICS_ASSERT();
	}
}

inline void Neptune::GLTextureCallsMapping::GLTexStorage(const Neptune::Texture::MetaData& _metaData)
{
	NEP_ASSERT(_metaData.m_mipmapLevels > 0); // Error: mipmap level is invalid
	
	switch (_metaData.m_type)
	{
		// S I N G L E   T E X T U R E S
	case Texture::Type::TEXTURE_1D:
		glTexStorage1D( GL_TEXTURE_1D,
		_metaData.m_mipmapLevels,
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width
		);
		break;

	case Texture::Type::TEXTURE_2D:
		glTexStorage2D(GL_TEXTURE_2D,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height);    
		break;

	case Texture::Type::TEXTURE_3D:
		glTexStorage3D(GL_TEXTURE_3D,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);    
		break;

	case Texture::Type::CUBE_MAP:
		GLTexStorageCubeMap(_metaData);
		break;

	case Texture::Type::BUFFER: // Not supported yet
		glTexBuffer(GL_TEXTURE_BUFFER,
			MapInternalBinaryFormat(_metaData.m_internalFormat),
			0); // A buffer object ID must be passed here -> a buffer object must be created first... Move buffers in a different class then?
		break;

		// A R R A Y S
	case Texture::Type::TEXTURE_ARRAY_1D:
		glTexStorage2D(GL_TEXTURE_1D_ARRAY,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height);   
		break;

	case Texture::Type::TEXTURE_ARRAY_2D:
		glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		_metaData.m_mipmapLevels,              
		MapInternalBinaryFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);  
		break;

	case Texture::Type::CUBE_MAP_ARRAY:
		GLTexStorageCubeMapArray(_metaData);
		break;
	}

	NEP_GRAPHICS_ASSERT();
}

inline void Neptune::GLTextureCallsMapping::GLTexSubImage(const Neptune::Texture::MetaData& _metaData, u8 _mipmapLevel, const u8** _data)
{
	switch (_metaData.m_type)
	{
		// S I N G L E   T E X T U R E S
	case Texture::Type::TEXTURE_1D:
		glTexSubImage1D(GL_TEXTURE_1D,
		_mipmapLevel,					// index of the current image in the mipmap list
		0,								// x offset
		_metaData.m_width,
		MapInternalColorFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		*_data);  
		break;

	case Texture::Type::TEXTURE_2D:
		glTexSubImage2D(GL_TEXTURE_2D,
		_mipmapLevel,		// index of the current image in the mipmap list
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalColorFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		*_data);  
		break;

	case Texture::Type::TEXTURE_3D:
		glTexSubImage3D(GL_TEXTURE_3D,
		_mipmapLevel,		// index of the current image in the mipmap list
		0,0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		_metaData.m_depth,
		MapInternalColorFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		*_data); 
		break;

	case Texture::Type::CUBE_MAP:
		GLTexSubImageCubeMap(_metaData, _mipmapLevel, *_data);
		break;

	case Texture::Type::BUFFER: 
		NEP_ASSERT(false); // Not supported yet
		break;

		// A R R A Y S
	case Texture::Type::TEXTURE_ARRAY_1D:
		for (u32 i = 0; i < _metaData.m_height; i++)
		{
			glTexSubImage2D(GL_TEXTURE_1D_ARRAY,
			_mipmapLevel,		// index of the current image in the mipmap list
			0,0,							// x,y offset
			_metaData.m_width,
			_metaData.m_height,
			MapInternalColorFormat(_metaData.m_internalFormat),
			GL_UNSIGNED_BYTE,
			_data[i]);

			NEP_GRAPHICS_ASSERT();
		}
		break;

	case Texture::Type::TEXTURE_ARRAY_2D:
		for (u32 i = 0; i < _metaData.m_depth; i++)
		{
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
			_mipmapLevel,		// index of the current image in the mipmap list
			0,0,0,							// x,y offset
			_metaData.m_width,
			_metaData.m_height,
			_metaData.m_depth,
			MapInternalColorFormat(_metaData.m_internalFormat),
			GL_UNSIGNED_BYTE,
			_data[i]); 

			NEP_GRAPHICS_ASSERT();
		}  
		break;

	case Texture::Type::CUBE_MAP_ARRAY:
		GLTexSubImageCubeMapArray(_metaData, _mipmapLevel, _data);
		break;
	}

	NEP_GRAPHICS_ASSERT();
}
