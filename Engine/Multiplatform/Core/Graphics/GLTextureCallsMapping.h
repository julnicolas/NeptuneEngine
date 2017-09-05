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
		GLenum			MapInternalFormat(Texture::InternalFormat _internalFormat);
		
		// Texture routine wrappers. Call the right OpenGL function with regard to the texture type contained in _metaData 
		void GLTexStorage(const Texture::MetaData& _metaData);
		void GLTexSubImage(const Texture::MetaData& _metaData, const void** _data);
		
		// Cube-map specifics
		void GLTexStorageCubeMap(const Texture::MetaData& _metaData);
		void GLTexStorageCubeMapArray(const Texture::MetaData& _metaData);
		void GLTexSubImageCubeMap(const Texture::MetaData& _metaData, const void* _data);
		void GLTexSubImageCubeMapArray(const Texture::MetaData& _metaData, const void** _data);
	}
}

#include "Graphics/GLTextureCallsMappingInlineImplementation.h"