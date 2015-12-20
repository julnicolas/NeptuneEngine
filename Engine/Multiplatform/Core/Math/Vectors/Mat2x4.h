#pragma once

namespace Neptune
{
	template <typename T> class BaseMatrix2x4;

	// Matrix with 2 rows and 4 columns
	template <typename T> class Mat2x4
	{
	public:
		Mat2x4<T>()                            = default;
		~Mat2x4<T>()                           = default;
		Mat2x4<T>(const Mat2x4<T>&)            = default;
		Mat2x4<T>& operator=(const Mat2x4<T>&) = default;

		Mat2x4<T>& operator*=(const Mat2x4<T>&);
		Mat2x4<T>& operator+=(const Mat2x4<T>&);
		Mat2x4<T>& operator-=(const Mat2x4<T>&);

		const T* getPtr() const;
		const BaseMatrix2x4<T>& getBase() const { return m_matrix; }

	private:
		BaseMatrix2x4<T> m_matrix; /// Depends on the platform
	};

	template <typename T> Mat2x4<T> operator*(const Mat2x4<T>&,const Mat2x4<T>&);
	template <typename T> Mat2x4<T> operator+(const Mat2x4<T>&,const Mat2x4<T>&);
	template <typename T> Mat2x4<T> operator-(const Mat2x4<T>&,const Mat2x4<T>&);
}

// Platform-specific Implementation
#include "Math/Vectors/Mat2x4Implementation.h" 