#pragma once

#include "Graphics/Texture.h"
#include "Debug/NeptuneDebug.h"

namespace Neptune
{
	namespace GLTextureCallsMapping
	{
		// Neptune <-> OpenGL Mapping functions
		u32				MapTextureType(Texture::Type _type);
		Texture::Type	MapTextureType(u32 _glType);
		GLenum			MapInternalColorFormat(Texture::InternalFormat _internalFormat);
		GLenum			MapInternalBinaryFormat(Texture::InternalFormat _internalFormat);
		
		// Texture routine wrappers. Call the right OpenGL function with regard to the texture type contained in _metaData 
		void GLTexStorage(const Texture::MetaData& _metaData);
		void GLTexSubImage(const Texture::MetaData& _metaData, u8 _mipmapLevel, const u8** _data);
		
		// Cube-map specifics
		void GLTexStorageCubeMap(const Texture::MetaData& _metaData);
		void GLTexStorageCubeMapArray(const Texture::MetaData& _metaData);
		void GLTexSubImageCubeMap(const Texture::MetaData& _metaData, u8 _mipmapLevel, const u8* _data);
		void GLTexSubImageCubeMapArray(const Texture::MetaData& _metaData, u8 _mipmapLevel, const u8** _data);
	}
}

#include "Graphics/GLTextureCallsMappingInlineImplementation.h"