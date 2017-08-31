#include "Graphics/Texture.h"
#include "STB/stb_image.h"
#include "Debug/NeptuneDebug.h"
#include "Graphics/IncludeOpenGL.h"

using namespace Neptune;


// Use the library FreeImage for loading pretty much all formats
// Add a ktx loader to be able to use this format.


////////////////////////////////////////////////////////////////
//
//						S T A T I C S
//
////////////////////////////////////////////////////////////////

static u16 MapTextureType(Texture::Type _type)
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

	case Texture::Type::MULTI_SAMPLE_2D:
		type = GL_TEXTURE_2D_MULTISAMPLE;
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

	case Texture::Type::TEXTURE_ARRAY_2D_MULTISAMPLE:
		type = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
		break;
	}

	return type;
}


////////////////////////////////////////////////////////////////
//
//			C L A S S   I M P L E M E N T A T I O N
//
////////////////////////////////////////////////////////////////

Texture::Texture():
	m_textureID(0),
	m_width(0), 
	m_height(0),
	m_depth(0),
	m_componentsPerPixel(0),
	m_path(nullptr),
	m_data(nullptr),
	m_size(0),
	m_type(Type::TEXTURE_2D)
{

}

Texture::Texture(const char* _path, Type _type /*= TextureType::TEXTURE_2D*/): 
	Texture()
{
	setPath(_path);
	m_type = _type;
}

Texture::Texture(void* _data, u32 _size, Type _type /*= TextureType::TEXTURE_2D*/):
	Texture()
{
	m_size = _size;
	m_data = _data;
	m_type = _type;
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
	m_size = _size;
}

void Texture::CreateTexture(u8* data, s32 reqComp)
{
	NEP_ASSERT(m_componentsPerPixel == reqComp);

	// Generate a name for the texture
	glGenTextures(1,&m_textureID);

	// Now bind to the graphics context
	glBindTexture(GL_TEXTURE_2D,m_textureID);

	// Specify texture's storage amount
	glTexStorage2D(GL_TEXTURE_2D,
		1,               // 1 Mipmap level
		GL_RGB32F,      // 32-bit RGBA floating point data
		m_width,       //width
		m_height);    // height

	// Copy image data to texture (the texture is assumed to be already bound)
	glTexSubImage2D(GL_TEXTURE_2D,
		0,    // Level of detail (mipmap relevant)
		0,0, // x,y offset
		m_width,
		m_height,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		data);

	// Free image data
	stbi_image_free(data);
	data = nullptr;
}

// Placeholder textures are magenta
void Texture::SetPlaceHolderTexture(u8*& data)
{
	m_width  = 1;
	m_height = 1;
	m_componentsPerPixel = 3;
	size_t size = m_width*m_height*m_componentsPerPixel;
	data = new u8[size];

	data[0] = 255;
	data[1] = 0;
	data[2] = 255;
}

bool Texture::init()
{
	// Reading image's content
	u8* data = stbi_load( m_path, (s32*) &m_width, (s32*) &m_height, (s32*) &m_componentsPerPixel, 3);
	if (data != nullptr)
	{
		CreateTexture( data, 3);
		return true;
	}
	else
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