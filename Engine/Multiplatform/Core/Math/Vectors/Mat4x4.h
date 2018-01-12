#pragma once

#include "Math/Vectors/Vec4.h"

namespace Neptune
{
	template <typename T> class BaseMatrix4x4;

	// Matrix with 4 rows and 4 columns
	template <typename T> class Mat4x4
	{
	public:
		Mat4x4<T>()                            = default;
		~Mat4x4<T>()                           = default;
		Mat4x4<T>(const Mat4x4<T>&)            = default;
		Mat4x4<T>& operator=(const Mat4x4<T>&) = default;

		Mat4x4<T>& operator*=(const Mat4x4<T>&);
		Mat4x4<T>& operator+=(const Mat4x4<T>&);
		Mat4x4<T>& operator-=(const Mat4x4<T>&);

		const T* getPtr() const;
		const BaseMatrix4x4<T>& getBase() const { return m_matrix; }

	private:
		BaseMatrix4x4<T> m_matrix; /// Depends on the platform
	};

	typedef Mat4x4<float> Mat4;

	// Matrix operators
	template <typename T> Mat4x4<T> operator*(const Mat4x4<T>&, const Mat4x4<T>&);
	template <typename T> Mat4x4<T> operator+(const Mat4x4<T>&, const Mat4x4<T>&);
	template <typename T> Mat4x4<T> operator-(const Mat4x4<T>&, const Mat4x4<T>&);

	// Vector/Scalar operators
	template <typename T> Vec4_t<T> operator*(const Mat4x4<T>&, const Vec4_t<T>&);
	template <typename T> Mat4x4<T> operator*(const Mat4x4<T>&, T);
	template <typename T> Mat4x4<T> operator*(T, const Mat4x4<T>&);
}

// Platform-specific Implementation
#include "Math/Vectors/Mat4Implementation.h" 