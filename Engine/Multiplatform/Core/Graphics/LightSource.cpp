#include "Graphics/LightSource.h"

using namespace Neptune;

LightSource::LightSource()
{
	// Default color to white
	m_color.r = 1.0f;
	m_color.g = 1.0f;
	m_color.b = 1.0f;

	// With max intensity
	m_color.a = 1.0f;
}

LightSource::~LightSource()
{

}