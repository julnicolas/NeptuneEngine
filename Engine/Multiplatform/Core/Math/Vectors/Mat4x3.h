#pragma once

namespace Neptune
{
	template <typename T> class BaseMatrix4x3;

	// Matrix with 4 rows and 3 columns
	template <typename T> class Mat4x3
	{
	public:
		Mat4x3<T>()                            = default;
		~Mat4x3<T>()                           = default;
		Mat4x3<T>(const Mat4x3<T>&)            = default;
		Mat4x3<T>& operator=(const Mat4x3<T>&) = default;

		Mat4x3<T>& operator*=(const Mat4x3<T>&);
		Mat4x3<T>& operator+=(const Mat4x3<T>&);
		Mat4x3<T>& operator-=(const Mat4x3<T>&);

		const T* getPtr() const;
		const BaseMatrix4x3<T>& getBase() const { return m_matrix; }

	private:
		BaseMatrix4x3<T> m_matrix; /// Depends on the platform
	};

	template <typename T> Mat4x3<T> operator*(const Mat4x3<T>&,const Mat4x3<T>&);
	template <typename T> Mat4x3<T> operator+(const Mat4x3<T>&,const Mat4x3<T>&);
	template <typename T> Mat4x3<T> operator-(const Mat4x3<T>&,const Mat4x3<T>&);
}

// Platform-specific Implementation
#include "Math/Vectors/Mat4x3Implementation.h" 