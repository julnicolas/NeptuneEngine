#pragma once

#include "Math/Vectors/Mat4.h"

namespace Neptune
{
	template <typename T> class Vec3; 

	class Transform
	{
	public:
		Transform();
		~Transform()                           = default;
		Transform(const Transform&)            = default;
		Transform& operator=(const Transform&) = default;

		void rotate(float x, float y, float z);
		void translate(float x, float y, float z);
		void scale(float x, float y, float z);

		void               getOrientation(Vec3<float>& orientation);
		void               getSize(Vec3<float>& size);
		void               getDisplacement(Vec3<float>& displacement);
		
		const float*       getDataPtr() const;
		const Mat4<float>& getMatrix()  const { return m_transform; }

	private:
		Mat4<float> m_transform;
		float       m_orientation[3];
		float       m_displacement[3];
		float       m_size[3];
	};

}