#include "Graphics/LightSource.h"

using namespace Neptune;

LightSource::LightSource(float x /*= 0.0f*/, float y /*= 0.0f*/, float z /*= 0.0f*/):
	m_position(x, y, z), m_baseIntensity(1.0f), 
	m_maxDistance(0), m_color({1.0f, 1.0f, 1.0f, 1.0f})
{

}

LightSource& LightSource::operator=(const LightSource& obj)
{
	m_maxDistance	=	obj.m_maxDistance;
	m_baseIntensity	=	obj.m_baseIntensity;
	m_position		=	obj.m_position;
	m_color			=	obj.m_color;
}