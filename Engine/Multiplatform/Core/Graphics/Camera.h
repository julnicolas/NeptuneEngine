#pragma once

#include "Math/Vectors/Mat4x4.h"
#include <functional>

namespace Neptune
{
	template <typename T> class Vec3_t;
	typedef Vec3_t<float> Vec3;

	class Camera
	{
	public:
		Camera();
		Camera(const Vec3& eye, const Vec3& center, const Vec3& up);  // Defines a look-at matrix
		~Camera()                        = default;
		Camera(const Camera&)            = default;
		Camera& operator=(const Camera&) = default;

		float getFieldOfView()            const { return m_fieldOfView;                                      }
		float getScreenRatio()            const { return m_screenRatio;                                      }
		float getNearPlan()               const { return m_nearPos;                                          }
		float getFarPlan()                const { return m_farPos;                                           }
		const Mat4& getProjectionMatrix() const { return m_projection;                                       }
		const Mat4& getViewMatrix()             { m_view = m_orientation*m_position*m_origin; return m_view; }
		const Mat4& getOrientation()	  const	{ return m_orientation;										 }

		void  setViewFrustum(float fieldOfView, float screenRatio, float nearPos, float farPos); // FOV in radians, near and far in world coord
		void  setScreenRatio(float ratio);
		void  toggleReversedZProjection();

		const Mat4& lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
		const Mat4& translate(float x, float y, float z);
		const Mat4& rotate(float angle_deg, const Vec3& axis); // Should be refactored using quaternions
		void  zoom(float k);                                  // Affects the field of view

		enum class ProjectionType
		{
			PERSPECTIVE,						/// Perspective projection bound by a near and far plan. This is the default projection.
			REVERSED_Z_PERSPECTIVE,				/// Perspective projection bound by a near and far plan. Must be used when reversed z off screen rendering is enabled.
			INFINITY_REVERSED_Z_PERSPECTIVE,	/// Perspective projection which projects the space from a near plan to the infinity. Works only if reversed z is enabled.
		};

		void updateProjection(ProjectionType _proj);

	private:
		void computeProjection();

		Mat4  m_origin;
		Mat4  m_orientation;
		Mat4  m_position;
		Mat4  m_view;
		Mat4  m_projection;
		std::function <decltype(m_projection)()> m_computeProjection; /// Active projection matrix generation function
		float m_fieldOfView;
		float m_screenRatio;
		float m_nearPos;    
		float m_farPos;
	};


}