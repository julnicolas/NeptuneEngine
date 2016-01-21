#include "Camera.h"
#include "Math/Vectors/Vec3.h"
#include "Math/Vectors/MatrixTransform.h"
#include "Math/Geometry/Trigonometry.h"
#include "Math/Math.h"

using namespace Neptune;

const float M_INIT_NEAR = 0.1f;
const float M_INIT_FAR  = 100.0f;
const float M_INIT_FOV  = Radians(45.0f);

Camera::Camera():
	m_fieldOfView( M_INIT_FOV ),
	m_screenRatio(16.0f/9.0f),
	m_nearPos(M_INIT_NEAR),
	m_farPos(M_INIT_FAR)
{
	setProjection();
}

Camera::Camera(const Vec3& eye, const Vec3& center, const Vec3& up):
	m_fieldOfView( M_INIT_FOV ),
	m_screenRatio(16.0f/9.0f),
	m_nearPos(M_INIT_NEAR),
	m_farPos(M_INIT_FAR)
{
	m_origin = LookAt(eye, center, up);
	setProjection();
}

void  Camera::setViewFrustum(float fieldOfView, float screenRatio, float nearPos, float farPos)
{
	m_fieldOfView = fieldOfView;
	m_screenRatio = screenRatio;
	m_nearPos     = nearPos;
	m_farPos      = farPos;

	setProjection();
}

void Camera::setScreenRatio(float ratio) 
{ 
	m_screenRatio = ratio;

	setProjection();
}

const Mat4& Camera::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
{
	m_origin      = LookAt(eye, center, up);

	// Reset position and orientation matrices
	m_orientation = Mat4();
	m_position    = Mat4();

	return m_origin;
}

const Mat4& Camera::translate(float x, float y, float z)
{
	Vec3 t(-x, -y, -z);
	m_position = Translate(m_position, t);

	return m_position;
}

const Mat4& Camera::rotate(float angle_deg, const Vec3& axis)
{
	m_orientation = Rotate(m_orientation, Radians(angle_deg), axis);

	return m_orientation;
}

void Camera::zoom(float k)
{
	if ( k > 0.0f && k <= 100.0f)
		m_fieldOfView = M_INIT_FOV * Max((100.0f-k)/100.0f, 0.01f);
	else
		m_fieldOfView = (k < 0.0f) ? M_INIT_FOV : m_fieldOfView;

	setProjection();
}

void Camera::setProjection()
{
	m_projection = Perspective( m_fieldOfView, m_screenRatio, m_nearPos, m_farPos );
}