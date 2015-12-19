#include "Math/Geometry/Transform.h"
#include "Math/Vectors/Vec3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string.h>

using namespace Neptune;

Transform::Transform()
{
	memset( m_orientation,  0, sizeof(m_orientation)  );
	memset( m_displacement, 0, sizeof(m_displacement) );
	memset( m_size,         0, sizeof(m_size)         );
}

void Transform::rotate(float x,float y,float z)
{
	glm::tmat4x4<float>* mat = &m_transform;

	if ( x != 0.0f )
	{
		float u          = glm::radians(x);
		m_orientation[0] += u;
		*mat = glm::rotate( m_transform, u, Vec3(1.0f, 0.0f, 0.0f) );
	}
	
	if ( y != 0.0f )
	{
		float v          = glm::radians(y);
		m_orientation[1] += v;
		*mat = glm::rotate( m_transform, v, Vec3(0.0f, 1.0f, 0.0f) );
	}
	
	if ( z != 0.0f )
	{
		float w          = glm::radians(z);
		m_orientation[2] += w;
		*mat = glm::rotate( m_transform, w, Vec3(0.0f, 0.0f, 1.0f) );
	}
}

void Transform::translate(float x,float y,float z)
{
	m_displacement[0] += x;
	m_displacement[1] += y;
	m_displacement[2] += z;

	glm::tmat4x4<float>* mat = &m_transform;
	*mat = glm::translate( m_transform, Vec3(x, y, z) );
}

void Transform::scale(float x,float y,float z)
{
	m_size[0] += x;
	m_size[1] += y;
	m_size[2] += z;

	glm::tmat4x4<float>* mat = &m_transform;
	*mat = glm::scale( m_transform, Vec3(x, y, z) );
}

void Transform::getOrientation(Vec3& orientation)
{
	orientation.x = glm::degrees( m_orientation[0] );
	orientation.y = glm::degrees( m_orientation[1] );
	orientation.z = glm::degrees( m_orientation[2] );
}

void Transform::getSize(Vec3& size)
{
	size.x = m_size[0];
	size.y = m_size[1];
	size.z = m_size[2];
}

void Transform::getDisplacement(Vec3& displacement)
{
	displacement.x = m_displacement[0];
	displacement.y = m_displacement[1];
	displacement.z = m_displacement[2];
}

const float* Transform::getDataPtr() const
{
	// Parent-type conversion to avoid a linker error which occurs
	// when Mat4<T> is used with the templated function glm::value_ptr
	const glm::tmat4x4<float>* mat = &m_transform;
	
	return glm::value_ptr( *mat );
}