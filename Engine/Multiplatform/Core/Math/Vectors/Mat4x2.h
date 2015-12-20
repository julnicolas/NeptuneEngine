#pragma once

namespace Neptune
{
	template <typename T> class BaseMatrix4x2;

	// Matrix with 4 rows and 2 columns
	template <typename T> class Mat4x2
	{
	public:
		Mat4x2<T>()                            = default;
		~Mat4x2<T>()                           = default;
		Mat4x2<T>(const Mat4x2<T>&)            = default;
		Mat4x2<T>& operator=(const Mat4x2<T>&) = default;

		Mat4x2<T>& operator*=(const Mat4x2<T>&);
		Mat4x2<T>& operator+=(const Mat4x2<T>&);
		Mat4x2<T>& operator-=(const Mat4x2<T>&);

		const T* getPtr() const;
		const BaseMatrix4x2<T>& getBase() const { return m_matrix; }

	private:
		BaseMatrix4x2<T> m_matrix; /// Depends on the platform
	};

	template <typename T> Mat4x2<T> operator*(const Mat4x2<T>&,const Mat4x2<T>&);
	template <typename T> Mat4x2<T> operator+(const Mat4x2<T>&,const Mat4x2<T>&);
	template <typename T> Mat4x2<T> operator-(const Mat4x2<T>&,const Mat4x2<T>&);
}

// Platform-specific Implementation
#include "Math/Vectors/Mat4x2Implementation.h" 