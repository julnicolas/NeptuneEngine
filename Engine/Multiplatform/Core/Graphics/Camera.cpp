#include "Camera.h"
#include "Math/Vectors/Vec3.h"
#include <glm/gtc/matrix_transform.hpp> // must be removed

using namespace Neptune;

Camera::Camera():
	m_fieldOfView( glm::radians(90.0f) ),
	m_screenRatio(16.0f/9.0f),
	m_nearPos(0.1f),
	m_farPos(10.0f)
{
	setProjection();
}

Camera::Camera(const Vec3<float>& eye, const Vec3<float>& center, const Vec3<float>& up):
	m_fieldOfView( glm::radians(90.0f) ),
	m_screenRatio(16.0f/9.0f),
	m_nearPos(0.1f),
	m_farPos(10.0f)
{
	glm::tmat4x4<float>* mat = &m_camera;
	*mat = glm::lookAt(eye, center, up);

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

const Mat4<float>& Camera::translate(const Vec3<float>& t)
{
	glm::tmat4x4<float>* mat = &m_camera;
	*mat *= glm::translate(m_camera, t);

	return m_camera;
}

const Mat4<float>& Camera::rotate(float angle_rad, const Vec3<float>& axis)
{
	glm::tmat4x4<float>* mat = &m_camera;
	*mat *= glm::rotate(m_camera, angle_rad, axis);

	return m_camera;
}

void Camera::zoom(float k)
{
	//m_fieldOfView
}

void Camera::setProjection()
{
	glm::tmat4x4<float>* mat = &m_projection;
	*mat = glm::perspective( m_fieldOfView, m_screenRatio, m_nearPos, m_farPos );
}