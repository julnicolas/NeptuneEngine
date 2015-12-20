#pragma once

namespace Neptune
{
	template <typename T> class BaseMatrix2x3;

	// Matrix with 2 rows and 3 columns
	template <typename T> class Mat2x3
	{
	public:
		Mat2x3<T>()                            = default;
		~Mat2x3<T>()                           = default;
		Mat2x3<T>(const Mat2x3<T>&)            = default;
		Mat2x3<T>& operator=(const Mat2x3<T>&) = default;

		Mat2x3<T>& operator*=(const Mat2x3<T>&);
		Mat2x3<T>& operator+=(const Mat2x3<T>&);
		Mat2x3<T>& operator-=(const Mat2x3<T>&);

		const T* getPtr() const;
		const BaseMatrix2x3<T>& getBase() const { return m_matrix; }

	private:
		BaseMatrix2x3<T> m_matrix; /// Depends on the platform
	};

	template <typename T> Mat2x3<T> operator*(const Mat2x3<T>&,const Mat2x3<T>&);
	template <typename T> Mat2x3<T> operator+(const Mat2x3<T>&,const Mat2x3<T>&);
	template <typename T> Mat2x3<T> operator-(const Mat2x3<T>&,const Mat2x3<T>&);
}

// Platform-specific Implementation
#include "Math/Vectors/Mat2x3Implementation.h" 