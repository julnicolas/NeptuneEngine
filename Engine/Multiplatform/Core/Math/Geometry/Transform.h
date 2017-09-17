#pragma once

#include "Math/Vectors/Mat4x4.h"

namespace Neptune
{
	template <typename T> class Vec3_t;
	typedef Vec3_t<float> Vec3;

	/// The world space is left-handed
	
	class Transform
	{
	public:
		Transform();
		~Transform()                           = default;
		Transform(const Transform&)            = default;
		Transform& operator=(const Transform&) = default;

		void rotate(float x, float y, float z); /// Rotates in a clockwise orientation. Angles in degrees.
		void translate(float x, float y, float z);
		void scale(float x, float y, float z);

		void getOrientation(Vec3& orientation);
		void getSize(Vec3& size);
		void getPosition(Vec3& displacement);
		
		const float* getDataPtr() const;
		const Mat4&  getMatrix()  const { return m_transform; }

	private:
		Mat4  m_transform;
		float m_orientation[3];
		float m_position[3];
		float m_size[3];
	};

}