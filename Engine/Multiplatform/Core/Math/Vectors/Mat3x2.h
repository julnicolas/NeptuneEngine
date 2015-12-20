#pragma once

namespace Neptune
{
	template <typename T> class BaseMatrix3x2;

	// Matrix with 4 rows and 3 columns
	template <typename T> class Mat3x2
	{
	public:
		Mat3x2<T>()                            = default;
		~Mat3x2<T>()                           = default;
		Mat3x2<T>(const Mat3x2<T>&)            = default;
		Mat3x2<T>& operator=(const Mat3x2<T>&) = default;

		Mat3x2<T>& operator*=(const Mat3x2<T>&);
		Mat3x2<T>& operator+=(const Mat3x2<T>&);
		Mat3x2<T>& operator-=(const Mat3x2<T>&);

		const T* getPtr() const;
		const BaseMatrix3x2<T>& getBase() const { return m_matrix; }

	private:
		BaseMatrix3x2<T> m_matrix; /// Depends on the platform
	};

	template <typename T> Mat3x2<T> operator*(const Mat3x2<T>&,const Mat3x2<T>&);
	template <typename T> Mat3x2<T> operator+(const Mat3x2<T>&,const Mat3x2<T>&);
	template <typename T> Mat3x2<T> operator-(const Mat3x2<T>&,const Mat3x2<T>&);
}

// Platform-specific Implementation
#include "Math/Vectors/Mat3x2Implementation.h" 