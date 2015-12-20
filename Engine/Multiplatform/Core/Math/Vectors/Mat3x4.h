#pragma once

namespace Neptune
{
	template <typename T> class BaseMatrix3x4;

	// Matrix with 4 rows and 3 columns
	template <typename T> class Mat3x4
	{
	public:
		Mat3x4<T>()                            = default;
		~Mat3x4<T>()                           = default;
		Mat3x4<T>(const Mat3x4<T>&)            = default;
		Mat3x4<T>& operator=(const Mat3x4<T>&) = default;

		Mat3x4<T>& operator*=(const Mat3x4<T>&);
		Mat3x4<T>& operator+=(const Mat3x4<T>&);
		Mat3x4<T>& operator-=(const Mat3x4<T>&);

		const T* getPtr() const;
		const BaseMatrix3x4<T>& getBase() const { return m_matrix; }

	private:
		BaseMatrix3x4<T> m_matrix; /// Depends on the platform
	};

	template <typename T> Mat3x4<T> operator*(const Mat3x4<T>&,const Mat3x4<T>&);
	template <typename T> Mat3x4<T> operator+(const Mat3x4<T>&,const Mat3x4<T>&);
	template <typename T> Mat3x4<T> operator-(const Mat3x4<T>&,const Mat3x4<T>&);
}

// Platform-specific Implementation
#include "Math/Vectors/Mat3x4Implementation.h" 