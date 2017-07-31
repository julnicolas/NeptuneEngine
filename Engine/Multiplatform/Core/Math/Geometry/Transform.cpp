#include "Math/Geometry/Transform.h"
#include "Math/Vectors/Vec3.h"
#include "Math/Vectors/MatrixTransform.h"
#include "Math/Geometry/Trigonometry.h"
#include <string.h>

using namespace Neptune;

Transform::Transform()
{
	memset( m_orientation,  0, sizeof(m_orientation)  );
	memset( m_position, 0, sizeof(m_position) );
	memset( m_size,         0, sizeof(m_size)         );
}

void Transform::rotate(float x,float y,float z)
{
	if ( x != 0.0f )
	{
		float u          = Radians(x);
		m_orientation[0] += u;
		m_transform = Rotate( m_transform, u, Vec3(1.0f, 0.0f, 0.0f) );
	}
	
	if ( y != 0.0f )
	{
		float v          = Radians(y);
		m_orientation[1] += v;
		m_transform = Rotate( m_transform, v, Vec3(0.0f, 1.0f, 0.0f) );
	}
	
	if ( z != 0.0f )
	{
		float w          = Radians(z);
		m_orientation[2] += w;
		m_transform = Rotate( m_transform, w, Vec3(0.0f, 0.0f, 1.0f) );
	}
}

void Transform::translate(float x,float y,float z)
{
	m_position[0] += x;
	m_position[1] += y;
	m_position[2] += z;

	m_transform = Translate( m_transform, Vec3(x, y, z) );
}

void Transform::scale(float x,float y,float z)
{
	m_size[0] += x;
	m_size[1] += y;
	m_size[2] += z;

	m_transform = Scale( m_transform, Vec3(x, y, z) );
}

void Transform::getOrientation(Vec3& orientation)
{
	orientation.x() = Degrees( m_orientation[0] );
	orientation.y() = Degrees( m_orientation[1] );
	orientation.z() = Degrees( m_orientation[2] );
}

void Transform::getSize(Vec3& size)
{
	size.x() = m_size[0];
	size.y() = m_size[1];
	size.z() = m_size[2];
}

void Transform::getPosition(Vec3& displacement)
{
	displacement.x() = m_position[0];
	displacement.y() = m_position[1];
	displacement.z() = m_position[2];
}

const float* Transform::getDataPtr() const
{	
	return m_transform.getPtr();
}