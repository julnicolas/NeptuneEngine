#pragma once

namespace Neptune
{
	template <typename T> class Vec3_t;
	template <typename T> class Mat4x4;

	template <typename T>
	Mat4x4<T> Translate(const Mat4x4<T>& matrix, const Vec3_t<T>& v);

	template <typename T>
	Mat4x4<T> Rotate(const Mat4x4<T>& matrix, float angle, const Vec3_t<T>& axis);

	template <typename T>
	Mat4x4<T> Scale(const Mat4x4<T>& matrix, const Vec3_t<T>& scale);

	template <typename T>
	Mat4x4<T> LookAt(const Vec3_t<T>& target, const Vec3_t<T>& center, const Vec3_t<T>& up);

	template <typename T>
	Mat4x4<T> Perspective(T fieldOfView, T screenRatio, T near, T far);

	template <typename T>
	Mat4x4<T> ReversedZPerspective(T fieldOfView, T screenRatio, T near, T far);

	template <typename T>
	Mat4x4<T> InfinityReversedZPerspective(T fieldOfView, T screenRatio, T near);

	template <typename T>
	Mat4x4<T> Transpose(const Mat4x4<T>& matrix);
}

#include "Math/Vectors/MatrixTransformImplementation.h"