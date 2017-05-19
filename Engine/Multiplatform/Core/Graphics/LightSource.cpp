#include "Graphics/LightSource.h"

using namespace Neptune;

#define DEFAULT_LIGHT_COLOR {1.0f, 1.0f, 1.0f, 1.0f}

LightSource::LightSource():
	m_baseIntensity(1.0f), 
	m_color(DEFAULT_LIGHT_COLOR)
{

}

LightSource::~LightSource()
{

}