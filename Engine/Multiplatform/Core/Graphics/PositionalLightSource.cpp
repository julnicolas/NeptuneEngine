#include "Graphics/PositionalLightSource.h"

using namespace Neptune;

static const u8 DEFAULT_MAX_DISTANCE = 10; // Not necessarily meaningful, but not null to be able to produce light

PositionalLightSource::PositionalLightSource(float _x /*= 0.0f*/,float _y /*= 0.0f*/,float _z /*= 0.0f*/):
	m_posX(_x),m_posY(_y),m_posZ(_z),
	m_maxDistance(DEFAULT_MAX_DISTANCE)
{

}

PositionalLightSource::~PositionalLightSource(){}

Vec3 PositionalLightSource::getPos() const 
{ 
	return Vec3(m_posX, m_posY, m_posZ); 
}

void PositionalLightSource::setPos(Vec3& pos)
{ 
	m_posX = pos.x();
	m_posY = pos.y();
	m_posZ = pos.z();
}