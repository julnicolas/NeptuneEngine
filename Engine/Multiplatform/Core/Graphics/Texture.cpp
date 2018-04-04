#include "Graphics/Texture.h"
#include "Graphics/IncludeOpenGL.h"
#include "Graphics/GLTextureCallsMapping.h"

#include "File/PathHelpers.h"
#include "Debug/NeptuneDebug.h"

#include "Libktx/ktx.h"
#include "STB/stb_image.h"

#include <string>
#include <algorithm>

using namespace Neptune;


/////////////////////////////////////////////////////////////////////////////////////////
//
//							L O A D   K T X   F I L E S
//
/////////////////////////////////////////////////////////////////////////////////////////

// Returns a texture target
const u32 LOAD_KTX_ERROR = ~0;
static u32 LoadAndCreateKTXTexture(const char* _path, u32* _textureID, Texture::MetaData* _metaData)
{
	NEP_ASSERT(_textureID != nullptr && _metaData != nullptr); // Error
	
	u32             gl_target   = 0,      gl_error = 0;
	KTX_dimensions  dimension   = {0};
	u8              is_mipmaped = false;
	
	// _textureID must be equal to 0
	NEP_ASSERT( *_textureID == 0 ); // Error bad initialisation
	KTX_error_code error = ktxLoadTextureN(_path, _textureID, &gl_target, &dimension, &is_mipmaped, &gl_error, nullptr, nullptr);
	
	if ( error != KTX_SUCCESS )
	{
		NEP_LOG("Error in LoadKTX : Opengl error code %u", gl_error);
		NEP_ASSERT(false);

		return LOAD_KTX_ERROR;
	}

	// Get texture dimension
	_metaData->m_width  = dimension.width;
	_metaData->m_height = dimension.height;
	_metaData->m_depth  = dimension.depth;

	// Get texture type
	_metaData->m_type = GLTextureCallsMapping::MapTextureType(gl_target);

	return gl_target;
}

static std::string GetExtension(const char* _path)
{
	std::string extension = PathHelpers::GetFileExtension(std::string(_path));
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower); 

	NEP_ASSERT(extension != ""); // Error! File name doesn't have an extension

	return extension;
}


////////////////////////////////////////////////////////////////
//
//						U T I L I T I E S
//
////////////////////////////////////////////////////////////////


static bool IsTextureInitialised(u32 _textureID)
{
	return _textureID != 0;
}

// Expects a C-array of data. The size is stored in Texture::MetaData
static void CreateTexture(u32& _textureID, const Texture::MetaData& _metaData, const u8** _data)
{
	NEP_ASSERT(_data != nullptr && *_data != nullptr);
	NEP_ASSERT(_metaData.m_type != Texture::Type::BUFFER); // Error: type not supported yet 

	// Generate a name for the texture
	glGenTextures(1,&_textureID);
	NEP_GRAPHICS_ASSERT();

	// Now bind to the graphics context
	glBindTexture( GLTextureCallsMapping::MapTextureType(_metaData.m_type), _textureID);
	NEP_GRAPHICS_ASSERT();

	// Specify texture's storage amount
	GLTextureCallsMapping::GLTexStorage(_metaData);

	// Copy image data to texture (the texture is assumed to be already bound)
	for (u8 i = 0; i < _metaData.m_mipmapLevels; i++)
		GLTextureCallsMapping::GLTexSubImage(_metaData, i,  _data);
}



////////////////////////////////////////////////////////////////
//
//					L O A D   I M A G E S
//
////////////////////////////////////////////////////////////////


static bool NEP_LoadImage(const char* _path, u32& _textureID, Texture::MetaData& _metaData)
{
	const s32 RGBA_BYTE_COUNT = 4;
	
	u8* data = stbi_load( _path, (s32*) (&_metaData.m_width), (s32*) (&_metaData.m_height), nullptr, RGBA_BYTE_COUNT);

	_metaData.m_internalFormat = Texture::InternalFormat::RGBA;
	_metaData.m_type           = Texture::Type::TEXTURE_2D; 
	_metaData.m_mipmapLevels   = 1;
	_metaData.m_size           = _metaData.m_width * _metaData.m_height * RGBA_BYTE_COUNT;
	
	// Error?
	if (data == nullptr)
	{
		NEP_ASSERT_ERR_MSG(data != nullptr, "File couldn't be found or format is not supported.\n\t"
											"Textures are expected to be stored in %s. A full path can also be provided.\n\t"
											"Path value is : %s", Texture::GetStandardDir().c_str(), _path); // path can't be nullptr because checked beforehand
		return false;
	}
	
	// MOVE DATA TO VRAM
	{
		const u8* d = data; // statement to avoid compiler warning
		CreateTexture( _textureID, _metaData, &d );
	}

	// Free image data
	stbi_image_free(data);

	return true;
}


////////////////////////////////////////////////////////////////
//
//			C L A S S   I M P L E M E N T A T I O N
//
////////////////////////////////////////////////////////////////

Texture::Texture():
	m_name(0),
	m_index(0),
	m_metaData({0}),
	m_path(nullptr)
{
	m_metaData.m_type			= Type::TEXTURE_2D;
	m_metaData.m_internalFormat	= InternalFormat::NOT_SUPPORTED;
}

Texture::Texture(const char* _path, Type _type /*= TextureType::TEXTURE_2D*/): 
	Texture()
{
	setPath(_path);
	m_metaData.m_type = _type;
}

Texture::Texture(Texture&& _texture)
{
	m_name		= _texture.m_name;
	m_index		= _texture.m_index;
	m_metaData	= _texture.m_metaData;
	m_path		= _texture.m_path;

	_texture.m_name							= 0;
	_texture.m_index						= 0;
	_texture.m_path							= nullptr;
	_texture.m_metaData.m_width				= 0;
	_texture.m_metaData.m_height			= 0;
	_texture.m_metaData.m_depth				= 0;
	_texture.m_metaData.m_size				= 0;
	_texture.m_metaData.m_mipmapLevels		= 0;
	_texture.m_metaData.m_type				= Type::NOT_SUPPORTED;
	_texture.m_metaData.m_internalFormat	= InternalFormat::NOT_SUPPORTED;
}

Texture& Texture::operator=(Texture&& _texture)
{
	m_name		= _texture.m_name;
	m_index		= _texture.m_index;
	m_metaData	= _texture.m_metaData;
	m_path		= _texture.m_path;

	_texture.m_name							= 0;
	_texture.m_index						= 0;
	_texture.m_path							= nullptr;
	_texture.m_metaData.m_width				= 0;
	_texture.m_metaData.m_height			= 0;
	_texture.m_metaData.m_depth				= 0;
	_texture.m_metaData.m_size				= 0;
	_texture.m_metaData.m_mipmapLevels		= 0;
	_texture.m_metaData.m_type				= Type::NOT_SUPPORTED;
	_texture.m_metaData.m_internalFormat	= InternalFormat::NOT_SUPPORTED;

	return *this;
}

Texture::~Texture()
{
	delete[] m_path;
}

void Texture::setPath(const char* _path)
{
	NEP_ASSERT(_path != nullptr); // Path is invalid

	const size_t length = strlen(_path) + 1;
	m_path              = new char[length];
	
	strcpy_s(m_path, length, _path);
}

void Texture::setIndex(u32 _index)
{
	NEP_ASSERT( _index <= getMaxTextureCount() ); // Error, index is invalid

	m_index = _index;
}

void Texture::setData(void* _data, u32 _size)
{
	NEP_ASSERT( _data != nullptr && _size > 0 ); // Wrong data

	m_metaData.m_size = _size;
}

bool Texture::init()
{
	// GET FILE EXTENSION
	NEP_ASSERT( m_path != nullptr && m_path[0] != '\0' ); // Error! Texture path is invalid
	
	if ( !IsTextureInitialised(m_name) )
	{
		std::string extension = GetExtension(m_path);

		// LOAD TEXTURE DATA
		if ( extension == ".ktx" )		// If it's a texture format
		{
			u32 texture_target = LoadAndCreateKTXTexture(m_path, &m_name, &m_metaData);

			NEP_ASSERT_ERR_MSG(texture_target != LOAD_KTX_ERROR, "Error, Texture couldn't be loaded. Path: %s", m_path);
			return texture_target != LOAD_KTX_ERROR;
		}
		else							// If it's an image format
		{
			return NEP_LoadImage(m_path, m_name, m_metaData);
		}
	}

	return false; // texture already initialised
}

bool Texture::update()
{
	// Check the texture has been initialised
	NEP_ASSERT(IsTextureInitialised(m_name));
	
	// Now activate the right texture index to be able to bind the texture
	glActiveTexture(GL_TEXTURE0 + m_index);
	
	// Now bind to the graphics context
	glBindTexture( GLTextureCallsMapping::MapTextureType(m_metaData.m_type), m_name);
	NEP_GRAPHICS_ASSERT();

	return m_name != 0;
}

void Texture::terminate()
{
	glDeleteTextures(1, &m_name);
	m_name = 0;
}

u32 Texture::getMaxTextureCount() const
{
	s32 max_count = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_count); // Never sets a negative value

	return static_cast<u32>(max_count);
}