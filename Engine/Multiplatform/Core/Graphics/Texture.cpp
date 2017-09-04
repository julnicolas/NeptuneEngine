#include "Graphics/Texture.h"
#include "STB/stb_image.h"
#include "Debug/NeptuneDebug.h"
#include "Graphics/IncludeOpenGL.h"
#include "File/PathHelpers.h"

#include "Libktx/ktx.h"

#include <string>
#include <algorithm>

using namespace Neptune;


////////////////////////////////////////////////////////////////
//
//				O P E N G L   M A P P I N G
//
////////////////////////////////////////////////////////////////

static u32 MapTextureType(Texture::Type _type)
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

static Texture::Type MapTextureType(u32 _glType)
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

	case GL_TEXTURE_CUBE_MAP:
		type = Texture::Type::CUBE_MAP;
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

	return type;
}

static GLenum MapInternalFormat(Texture::InternalFormat _internalFormat)
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

//! To use this function, openGL 4.3 or ARB_texture_storage must be available
static void GLTexStorageCubeMap(const Texture::MetaData& _metaData)
{
	NEP_ASSERT(_metaData.m_width == _metaData.m_height); // Error the texture is not a square
	
	glTexStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height); 

	glTexStorage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height); 

	glTexStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height); 

	glTexStorage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height); 

	glTexStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height); 

	glTexStorage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height); 
}

static void GLTexStorageCubeMapArray(const Texture::MetaData& _metaData)
{
	NEP_ASSERT(_metaData.m_width == _metaData.m_height); // Error the texture is not a square
	NEP_ASSERT(_metaData.m_depth > 0); // Warning: You probably don't want an array

	NEP_ASSERT(false); // gl enum is not correct fix it man

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);  

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);  

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);  

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);  

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);  

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);  
}

static void GLTexSubImageCubeMap(const Texture::MetaData& _metaData, const void* _data)
{
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data);

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data);  

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data);  

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data);  

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data);  

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data);  
}

static void GLTexSubImageCubeMapArray(const Texture::MetaData& _metaData, const void** _data)
{
	NEP_ASSERT(false); // gl enum is probably not correct
	
	for (u32 i = 0; i < _metaData.m_depth; i++)
	{
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data[i]);

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data[i]);  

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data[i]);  

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data[i]);  

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data[i]);  

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		_data[i]);
	}
}

static void GLTexStorage(const Texture::MetaData& _metaData)
{
	switch (_metaData.m_type)
	{
		// S I N G L E   T E X T U R E S
	case Texture::Type::TEXTURE_1D:
		glTexStorage1D( GL_TEXTURE_1D,
		_metaData.m_mipmapLevel,
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width
		);
		break;

	case Texture::Type::TEXTURE_2D:
		glTexStorage2D(GL_TEXTURE_2D,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height);    
		break;

	case Texture::Type::TEXTURE_3D:
		glTexStorage3D(GL_TEXTURE_3D,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);    
		break;

	case Texture::Type::CUBE_MAP:
		GLTexStorageCubeMap(_metaData);
		break;

	case Texture::Type::BUFFER: // Not supported yet
		glTexBuffer(GL_TEXTURE_BUFFER,
			MapInternalFormat(_metaData.m_internalFormat),
			0); // A buffer object ID must be passed here -> a buffer object must be created first... Move buffers in a different class then?
		break;

		// A R R A Y S
	case Texture::Type::TEXTURE_ARRAY_1D:
		glTexStorage2D(GL_TEXTURE_1D_ARRAY,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height);   
		break;

	case Texture::Type::TEXTURE_ARRAY_2D:
		glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		_metaData.m_mipmapLevel,              
		MapInternalFormat(_metaData.m_internalFormat),
		_metaData.m_width,       
		_metaData.m_height,
		_metaData.m_depth);  
		break;

	case Texture::Type::CUBE_MAP_ARRAY:
		GLTexStorageCubeMapArray(_metaData);
		break;
	}
}

static void GLTexSubImage(const Texture::MetaData& _metaData, const void** _data)
{
	switch (_metaData.m_type)
	{
		// S I N G L E   T E X T U R E S
	case Texture::Type::TEXTURE_1D:
		glTexSubImage1D(GL_TEXTURE_1D,
		_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,								// x offset
		_metaData.m_width,
		MapInternalFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		*_data);  
		break;

	case Texture::Type::TEXTURE_2D:
		glTexSubImage2D(GL_TEXTURE_2D,
		_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		MapInternalFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		*_data);  
		break;

	case Texture::Type::TEXTURE_3D:
		glTexSubImage3D(GL_TEXTURE_3D,
		_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
		0,0,0,							// x,y offset
		_metaData.m_width,
		_metaData.m_height,
		_metaData.m_depth,
		MapInternalFormat(_metaData.m_internalFormat),
		GL_UNSIGNED_BYTE,
		*_data); 
		break;

	case Texture::Type::CUBE_MAP:
		GLTexSubImageCubeMap(_metaData, *_data);
		break;

	case Texture::Type::BUFFER: 
		NEP_ASSERT(false); // Not supported yet
		break;

		// A R R A Y S
	case Texture::Type::TEXTURE_ARRAY_1D:
		for (u32 i = 0; i < _metaData.m_height; i++)
		{
			glTexSubImage2D(GL_TEXTURE_1D_ARRAY,
			_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
			0,0,							// x,y offset
			_metaData.m_width,
			_metaData.m_height,
			MapInternalFormat(_metaData.m_internalFormat),
			GL_UNSIGNED_BYTE,
			_data[i]);
		}
		break;

	case Texture::Type::TEXTURE_ARRAY_2D:
		for (u32 i = 0; i < _metaData.m_depth; i++)
		{
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
			_metaData.m_mipmapLevel,		// Level of detail (mipmap relevant)
			0,0,0,							// x,y offset
			_metaData.m_width,
			_metaData.m_height,
			_metaData.m_depth,
			MapInternalFormat(_metaData.m_internalFormat),
			GL_UNSIGNED_BYTE,
			_data[i]); 
		}  
		break;

	case Texture::Type::CUBE_MAP_ARRAY:
		GLTexSubImageCubeMapArray(_metaData, _data);
		break;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//							L O A D   K T X   F I L E S
//
/////////////////////////////////////////////////////////////////////////////////////////

// Returns a texture target
const u32 LOAD_KTX_ERROR = ~0;
static u32 LoadKTX(const char* _path, u32* _textureID, Texture::MetaData* _metaData)
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
	_metaData->m_type   = MapTextureType(gl_target);

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

void Texture::CreateTexture(u8* _data, s32 reqComp)
{
	NEP_ASSERT(m_metaData.m_type != Type::BUFFER); // Error: type not supported yet 
	//NEP_ASSERT(m_metaData.m_internalFormat == reqComp);

	// Generate a name for the texture
	glGenTextures(1,&m_textureID);

	// Now bind to the graphics context
	glBindTexture( MapTextureType(m_metaData.m_type), m_textureID);

	// Specify texture's storage amount
	GLTexStorage(m_metaData);

	// Copy image data to texture (the texture is assumed to be already bound)
	GLTexSubImage(m_metaData, (const void**) (&_data));

	// Free image data
	stbi_image_free(_data);
	_data = nullptr;
}

// Placeholder textures are magenta
void Texture::SetPlaceHolderTexture(u8*& data)
{
	m_metaData.m_width  = 1;
	m_metaData.m_height = 1;
	m_metaData.m_internalFormat = Texture::InternalFormat::RGB;

	const u8 INTERNAL_FORMAT_SIZE = 3;
	size_t size = m_metaData.m_width*m_metaData.m_height*INTERNAL_FORMAT_SIZE;
	data = new u8[size];

	data[0] = 255;
	data[1] = 0;
	data[2] = 255;
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
		texture_target = LoadKTX(m_path, &m_textureID, &m_metaData);
	else							// If it's an image format
		data = stbi_load( m_path, (s32*) &m_metaData.m_width, (s32*) &m_metaData.m_height, (s32*) &m_metaData.m_internalFormat, 3);
	
	// MOVE DATA TO VRAM
	// Reading image's content
	if (data != nullptr && texture_target != LOAD_KTX_ERROR)
	{
		CreateTexture( data, 3);
		return true;
	}
	else // We tolerate textures not being loaded, for instance if the art hasn't had time to make them 
	{
		SetPlaceHolderTexture(data);
		CreateTexture(data, 3);
		return false;
	}
}

bool Texture::update()
{
	// Now bind to the graphics context
	glBindTexture( GL_TEXTURE_2D, m_textureID);

	return m_textureID != 0;
}

void Texture::terminate()
{
	glDeleteTextures(1, &m_textureID);
	m_textureID = 0;
}