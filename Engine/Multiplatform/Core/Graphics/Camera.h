#pragma once

//#include <glm/gtc/matrix_transform.hpp>
#include "Math/Vectors/Mat4.h"

namespace Neptune
{
	template <typename T> class Vec3;

	class Camera
	{
	public:
		Camera();
		Camera(const Vec3<float>& eye, const Vec3<float>& center, const Vec3<float>& up);  // Defines a look-at matrix
		~Camera()                        = default;
		Camera(const Camera&)            = default;
		Camera& operator=(const Camera&) = default;

		float getFieldOfView()  const                  { return m_fieldOfView; }
		float getScreenRatio()  const                  { return m_screenRatio; }
		float getNearPlan()     const                  { return m_nearPos;     }
		float getFarPlan()      const                  { return m_farPos;      }
		const Mat4<float>& getViewMatrix()       const { return m_camera;      }
		const Mat4<float>& getProjectionMatrix() const { return m_projection;  }

		void  setViewFrustum(float fieldOfView, float screenRatio, float nearPos, float farPos); // FOV in radians, near and far in world coord
		void  setScreenRatio(float ratio);

		const Mat4<float>& translate(const Vec3<float>& t);
		const Mat4<float>& rotate(float angle_rad, const Vec3<float>& axis); // Should be refactored using quaternions
		void  zoom(float k);                                                // Affects field of view

	private:
		void setProjection();

		Mat4<float> m_camera;
		Mat4<float> m_projection;
		float		m_fieldOfView;
		float		m_screenRatio;
		float		m_nearPos;    
		float		m_farPos;     
	};


}