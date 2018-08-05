#pragma once

#include "Math/Vectors/Mat4x4.h"
#include "Math/Vectors/Vec3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>

namespace Neptune
{
	template <typename T>
	Mat4x4<T> Translate(const Mat4x4<T>& matrix,const Vec3_t<T>& vec)
	{
		const glm::tmat4x4<T>* m = &matrix.getBase();
		const glm::tvec3<T>*   v = &vec.getBase();
		Mat4x4<T> r;

		glm::tmat4x4<T>& base = const_cast<glm::tmat4x4<T>&>( static_cast<const glm::tmat4x4<T>&>( r.getBase() ));
		base = glm::translate( *m, *v);

		return r;
	}

	template <typename T>
	Mat4x4<T> Rotate(const Mat4x4<T>& matrix,float angle,const Vec3_t<T>& axis)
	{
		const glm::tmat4x4<T>* m = &matrix.getBase();
		const glm::tvec3<T>*   a = &axis.getBase();
		Mat4x4<T> r;

		glm::tmat4x4<T>& base = const_cast<glm::tmat4x4<T>&>( static_cast<const glm::tmat4x4<T>&>( r.getBase() ));
		base = glm::rotate( *m, -angle, *a ); // Rotation of -angle to turn in a clockwise orientation

		return r;
	}

	template <typename T>
	Mat4x4<T> Scale(const Mat4x4<T>& matrix,const Vec3_t<T>& scale)
	{
		const glm::tmat4x4<T>* m = &matrix.getBase();
		const glm::tvec3<T>*   s = &scale.getBase();
		Mat4x4<T> r;

		glm::tmat4x4<T>& base = const_cast<glm::tmat4x4<T>&>( static_cast<const glm::tmat4x4<T>&>( r.getBase() ));
		base = glm::scale(*m,*s);

		return r;
	}

	template <typename T>
	Mat4x4<T> LookAt(const Vec3_t<T>& eye,const Vec3_t<T>& center,const Vec3_t<T>& up)
	{
		const glm::tvec3<T>&   e      = const_cast<glm::tvec3<T>&>( static_cast<const glm::tvec3<T>&>( eye.getBase()    ));
		const glm::tvec3<T>&   c      = const_cast<glm::tvec3<T>&>( static_cast<const glm::tvec3<T>&>( center.getBase() ));
		const glm::tvec3<T>&   u_base = const_cast<glm::tvec3<T>&>( static_cast<const glm::tvec3<T>&>( up.getBase()     ));

		glm::tvec3<T> const f(glm::normalize(c - e));
		glm::tvec3<T> const s(-glm::normalize(glm::cross(f,u_base)));
		glm::tvec3<T> const u(-glm::cross(s,f));

		Mat4x4<T> r;
		glm::tmat4x4<T>& Result = const_cast<glm::tmat4x4<T>&>( static_cast<const glm::tmat4x4<T>&>( r.getBase() ));
		Result[0][0] = s.x;
		Result[1][0] = s.y;
		Result[2][0] = s.z;
		Result[0][1] = u.x;
		Result[1][1] = u.y;
		Result[2][1] = u.z;
		Result[0][2] = f.x;
		Result[1][2] = f.y;
		Result[2][2] = f.z;
		Result[3][0] = glm::dot(s,e);
		Result[3][1] = glm::dot(u,e);
		Result[3][2] =-glm::dot(f,e);

		return r;
	}

	template <typename T>
	Mat4x4<T> Perspective(T fieldOfView,T screenRatio,T near,T far)
	{
		Mat4x4<T> r;
		glm::tmat4x4<T>& base = const_cast<glm::tmat4x4<T>&>( static_cast<const glm::tmat4x4<T>&>( r.getBase() ));
		base = glm::perspective(fieldOfView, screenRatio, near, far);

		// Cancel the frame orientation change made by glm::perspective
		base[2] *= -1;

		return r;
	}

	template <typename T>
	Mat4x4<T> ReversedZPerspective(T fieldOfView, T screenRatio, T near, T far)
	{
		Mat4x4<T> r = Perspective(fieldOfView, screenRatio, near, far);
		Mat4x4<T> reverse_z(
			1, 0,  0, 0, 
			0, 1,  0, 0,
			0, 0, -1, 0,
			0, 0,  1, 1
			);

		return reverse_z * r;
	}

	template <typename T>
	Mat4x4<T> InfinityReversedZPerspective(T fieldOfView, T screenRatio, T near)
	{
		// reversed-z-matrix with far plane to infinity
		float f = 1.0f / tan(fieldOfView / 2.0f);
		Mat4x4<T> proj
			(f / screenRatio,	0.0f,	0.0f,		0.0f,
			0.0f,				f,		0.0f,		0.0f,
			0.0f,				0.0f,	0.0f,		1.0f,
			0.0f,				0.0f,	near,		0.0f);
		
		return proj;
	}

	template <typename T>
	Mat4x4<T> Transpose(const Mat4x4<T>& matrix)
	{
		Mat4x4<T> r;
		glm::tmat4x4<T>& base = const_cast<glm::tmat4x4<T>&>( static_cast<const glm::tmat4x4<T>&>( r.getBase() ));
		base = glm::transpose( matrix.getBase() );

		return r;
	}
}