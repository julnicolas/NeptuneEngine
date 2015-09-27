#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace Neptune
{
	class Camera
	{
	public:
		Camera();
		Camera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);  // Defines a look-at matrix
		~Camera()                        = default;
		Camera(const Camera&)            = default;
		Camera& operator=(const Camera&) = default;

		float getFieldOfView()  const  { return m_fieldOfView; }
		float getScreenRatio()  const  { return m_screenRatio; }
		float getNearPlan()     const  { return m_nearPos;     }
		float getFarPlan()      const  { return m_farPos;      }
		const glm::mat4& getMatrix() const  { return m_camera;      }

		void  setViewFrustum(float fieldOfView, float screenRatio, float nearPos, float farPos); // FOV in radians, near and far in world coord
		void  setScreenRatio(float ratio) { m_screenRatio = ratio; }

		const glm::mat4& translate(const glm::vec3& t);
		const glm::mat4& rotate(float angle, const glm::vec3& axis); // Should be refactored using quaternions
		void  zoom(float k);                          // Affects field of view

	private:
		glm::mat4  m_camera;
		float m_fieldOfView;
		float m_screenRatio;
		float m_nearPos;    
		float m_farPos;     
	};


}