#include "Graphics/Texture.h"
#include "STB/stb_image.h"
#include "Debug/NeptuneDebug.h"
#include "Graphics/IncludeOpenGL.h"

using namespace Neptune;

Texture::Texture(const char* name): 
	m_textureID(0),
	m_width(0), 
	m_height(0),
	m_componentsPerPixel(0)
{
	if ( name != nullptr )
	{
		m_name = new char[ strlen(name) ];
		strcpy(m_name, name);
	}
	else
	{
		m_name  = new char[1];
		*m_name = '\0';
	}
}

Texture::~Texture()
{
	delete[] m_name;
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
	u8* data = stbi_load( m_name, &m_width, &m_height, &m_componentsPerPixel, 3);
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