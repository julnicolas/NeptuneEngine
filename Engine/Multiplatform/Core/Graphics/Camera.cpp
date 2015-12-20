#include "Camera.h"
#include "Math/Vectors/Vec3.h"
#include "Math/Vectors/MatrixTransform.h"
#include "Math/Geometry/Trigonometry.h"

using namespace Neptune;

Camera::Camera():
	m_fieldOfView( Radians(90.0f) ),
	m_screenRatio(16.0f/9.0f),
	m_nearPos(0.1f),
	m_farPos(10.0f)
{
	setProjection();
}

Camera::Camera(const Vec3& eye, const Vec3& center, const Vec3& up):
	m_fieldOfView( Radians(90.0f) ),
	m_screenRatio(16.0f/9.0f),
	m_nearPos(0.1f),
	m_farPos(10.0f)
{
	m_camera = LookAt(eye, center, up);
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

const Mat4& Camera::translate(const Vec3& t)
{
	m_camera *= Translate(m_camera, t);

	return m_camera;
}

const Mat4& Camera::rotate(float angle_rad, const Vec3& axis)
{
	m_camera *= Rotate(m_camera, angle_rad, axis);

	return m_camera;
}

void Camera::zoom(float k)
{
	//m_fieldOfView
}

void Camera::setProjection()
{
	m_projection = Perspective( m_fieldOfView, m_screenRatio, m_nearPos, m_farPos );
}