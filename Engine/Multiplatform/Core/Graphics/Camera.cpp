#include "Camera.h"

using namespace Neptune;

Camera::Camera():
	m_camera(1),
	m_fieldOfView( glm::radians(45.0f) ),
	m_screenRatio(16.0f/9.0f),
	m_nearPos(0.1f),
	m_farPos(10.0f)
{}

Camera::Camera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up):
	m_fieldOfView( glm::radians(45.0f) ),
	m_screenRatio(16.0f/9.0f),
	m_nearPos(0.1f),
	m_farPos(10.0f)
{
	m_camera = glm::lookAt(eye, center, up);
}

void  Camera::setViewFrustum(float fieldOfView, float screenRatio, float nearPos, float farPos)
{
	m_fieldOfView = fieldOfView;
	m_screenRatio = screenRatio;
	m_nearPos     = nearPos;
	m_farPos      = farPos;
}

const glm::mat4& Camera::translate(const glm::vec3& t)
{
	m_camera *= glm::translate(m_camera, t);

	return m_camera;
}

const glm::mat4& Camera::rotate(float angle_rad, const glm::vec3& axis)
{
	m_camera *= glm::rotate(m_camera, angle_rad, axis);

	return m_camera;
}

void Camera::zoom(float k)
{
	//m_fieldOfView
}