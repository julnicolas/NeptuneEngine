#pragma once

#include "Math/Vectors/Mat4x4.h"
#include "Math/Vectors/Vec3.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Neptune
{
	template <typename T>
	Mat4x4<T> Translate(const Mat4x4<T>& matrix,const Vec3_t<T>& vec)
	{
		const glm::tmat4x4<T>* m = &matrix.getBase();
		const glm::tvec3<T>*   v = &vec.getBase();
		Mat4x4<T> r;

		(glm::tmat4x4<T>&) r.getBase() = glm::translate( *m, *v);

		return r;
	}

	template <typename T>
	Mat4x4<T> Rotate(const Mat4x4<T>& matrix,float angle,const Vec3_t<T>& axis)
	{
		const glm::tmat4x4<T>* m = &matrix.getBase();
		const glm::tvec3<T>*   a = &axis.getBase();
		Mat4x4<T> r;

		// Rotation of -angle to turn in a clockwise orientation
		(glm::tmat4x4<T>&) r.getBase() = glm::rotate( *m, -angle, *a );

		return r;
	}

	template <typename T>
	Mat4x4<T> Scale(const Mat4x4<T>& matrix,const Vec3_t<T>& scale)
	{
		const glm::tmat4x4<T>* m = &matrix.getBase();
		const glm::tvec3<T>*   s = &scale.getBase();
		Mat4x4<T> r;

		(glm::tmat4x4<T>&) r.getBase() = glm::scale(*m,*s);

		return r;
	}

	template <typename T>
	Mat4x4<T> LookAt(const Vec3_t<T>& target,const Vec3_t<T>& center,const Vec3_t<T>& up)
	{
		const glm::tvec3<T>*   t = &target.getBase();
		const glm::tvec3<T>*   c = &center.getBase();
		const glm::tvec3<T>*   u = &up.getBase();
		Mat4x4<T> r;

		(glm::tmat4x4<T>&) r.getBase() = glm::lookAt( *t, *c, *u );

		return r;
	}

	template <typename T>
	Mat4x4<T> Perspective(T fieldOfView,T screenRatio,T near,T far)
	{
		Mat4x4<T> r;
		(glm::tmat4x4<T>&) r.getBase() = glm::perspective(fieldOfView, screenRatio, near, far);

		return r;
	}
}