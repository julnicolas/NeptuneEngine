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
	_metaData->m_type   = GLTextureCallsMapping::MapTextureType(gl_target);

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
//			C L A S S   I M P L E M E N T A T I O N
//
////////////////////////////////////////////////////////////////

Texture::Texture():
	m_textureID(0),
	m_metaData({0}),
	m_path(nullptr),
	m_data(nullptr)
{
	m_metaData.m_type = Type::TEXTURE_2D;
}

Texture::Texture(const char* _path, Type _type /*= TextureType::TEXTURE_2D*/): 
	Texture()
{
	setPath(_path);
	m_metaData.m_type = _type;
}

Texture::Texture(void* _data, u32 _size, Type _type /*= TextureType::TEXTURE_2D*/):
	Texture()
{
	m_metaData.m_size = _size;
	m_metaData.m_type = _type;
	m_data = _data;
}

Texture::~Texture()
{
	delete[] m_path;
}

void Texture::setPath(const char* _path)
{
	NEP_ASSERT(_path != nullptr); // Path is invalid

	m_path = new char[ strlen(_path) ];
	strcpy(m_path, _path);
}

void Texture::setData(void* _data, u32 _size)
{
	NEP_ASSERT( _data != nullptr && _size > 0 ); // Wrong data

	m_data = _data;
	m_metaData.m_size = _size;
}

void Texture::CreateTexture(u8* _data)
{
	NEP_ASSERT(m_metaData.m_type != Type::BUFFER); // Error: type not supported yet 

	// Generate a name for the texture
	glGenTextures(1,&m_textureID);
	auto error = glGetError();
	NEP_ASSERT(error == GL_NO_ERROR);

	// Now bind to the graphics context
	glBindTexture( GL_TEXTURE_2D /*GLTextureCallsMapping::MapTextureType(m_metaData.m_type)*/, m_textureID);
	error = glGetError();
	NEP_ASSERT(error == GL_NO_ERROR);

	// Specify texture's storage amount
	//GLTextureCallsMapping::GLTexStorage(m_metaData);

	glTexStorage2D(GL_TEXTURE_2D,
		1,//m_metaData.m_mipmapLevel,              
		GL_RGBA32F,//MapInternalFormat(m_metaData.m_internalFormat),
		m_metaData.m_width,       
		m_metaData.m_height); 

	error = glGetError();
	NEP_ASSERT(error == GL_NO_ERROR);

	// Copy image data to texture (the texture is assumed to be already bound)
	//GLTextureCallsMapping::GLTexSubImage(m_metaData, (const void**) (&_data));

	glTexSubImage2D(GL_TEXTURE_2D,
		0,//m_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,0,							// x,y offset
		m_metaData.m_width,
		m_metaData.m_height,
		GL_RGBA,//GLTextureCallsMapping::MapInternalFormat(m_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data); 
	
	error = glGetError();
	NEP_ASSERT(error == GL_NO_ERROR);

	// Free image data
	stbi_image_free(_data);
	_data = nullptr;
}

bool Texture::init()
{
	// GET FILE EXTENSION
	NEP_ASSERT( m_path != nullptr && m_path[0] != '\0' ); // Error! Texture path is invalid
	std::string extension = GetExtension(m_path);

	// LOAD TEXTURE DATA
	u8* data = nullptr;
	u32 texture_target = 0;
	if ( extension == ".ktx" )		// If it's a texture format
	{
		texture_target = LoadAndCreateKTXTexture(m_path, &m_textureID, &m_metaData);

		NEP_ASSERT(texture_target != LOAD_KTX_ERROR); // Error, texture couldn't be loaded.
		return texture_target != LOAD_KTX_ERROR;
	}
	else							// If it's an image format
	{
		const s32 RGBA_BYTE_COUNT = 4;
		
		data = stbi_load( m_path, (s32*) &m_metaData.m_width, (s32*) &m_metaData.m_height, nullptr, RGBA_BYTE_COUNT);

		m_metaData.m_internalFormat = Texture::InternalFormat::RGB;
		m_metaData.m_type           = Texture::Type::TEXTURE_2D; 
		m_metaData.m_mipmapLevel    = 1;
		m_metaData.m_size           = m_metaData.m_width* m_metaData.m_height * RGBA_BYTE_COUNT;
	}
	
	// Error?
	if (data == nullptr)
	{
		NEP_ASSERT(false); // File probably doesn't exist or format is not supported
		return false;
	}
	
	// MOVE DATA TO VRAM
	CreateTexture( data );
	return true;
}

bool Texture::update()
{
	// Now bind to the graphics context
	glBindTexture( GLTextureCallsMapping::MapTextureType(m_metaData.m_type), m_textureID);

	return m_textureID != 0;
}

void Texture::terminate()
{
	glDeleteTextures(1, &m_textureID);
	m_textureID = 0;
}