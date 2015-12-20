#pragma once

namespace Neptune
{
	template <typename T> class BaseMatrix3x3;

	// Matrix with 4 rows and 4 columns
	template <typename T> class Mat3x3
	{
	public:
		Mat3x3<T>()                            = default;
		~Mat3x3<T>()                           = default;
		Mat3x3<T>(const Mat3x3<T>&)            = default;
		Mat3x3<T>& operator=(const Mat3x3<T>&) = default;

		Mat3x3<T>& operator*=(const Mat3x3<T>&);
		Mat3x3<T>& operator+=(const Mat3x3<T>&);
		Mat3x3<T>& operator-=(const Mat3x3<T>&);

		const T* getPtr() const;
		const BaseMatrix3x3<T>& getBase() const { return m_matrix; }

	private:
		BaseMatrix3x3<T> m_matrix; /// Depends on the platform
	};

	typedef Mat3x3<float> Mat3;

	template <typename T> Mat3x3<T> operator*(const Mat3x3<T>&,const Mat3x3<T>&);
	template <typename T> Mat3x3<T> operator+(const Mat3x3<T>&,const Mat3x3<T>&);
	template <typename T> Mat3x3<T> operator-(const Mat3x3<T>&,const Mat3x3<T>&);
}

// Platform-specific Implementation
#include "Math/Vectors/Mat4Implementation.h" 