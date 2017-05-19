#include "Graphics/DirectionalLightSource.h"

using namespace Neptune;

DirectionalLightSource::DirectionalLightSource(Vec3& _direction,float _x /*= 0.0f*/,float _y /*= 0.0f*/,float _z /*= 0.0f*/):
	PositionalLightSource(_x,_y,_z)
{
	m_dirX = _direction.x();
	m_dirY = _direction.y();
	m_dirZ = _direction.z();
}

DirectionalLightSource::~DirectionalLightSource(){}

Vec3 DirectionalLightSource::getDirection() const
{
	return Vec3(m_dirX, m_dirY, m_dirZ);
}

void DirectionalLightSource::setDirection(Vec3& _dir)
{
	m_dirX = _dir.x();
	m_dirY = _dir.y();
	m_dirZ = _dir.z();
}