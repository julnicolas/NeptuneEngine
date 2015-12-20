#pragma once

namespace Neptune
{
	template <typename T> class BaseMatrix2x2;

	// Matrix with 4 rows and 4 columns
	template <typename T> class Mat2x2
	{
	public:
		Mat2x2<T>()                            = default;
		~Mat2x2<T>()                           = default;
		Mat2x2<T>(const Mat2x2<T>&)            = default;
		Mat2x2<T>& operator=(const Mat2x2<T>&) = default;

		Mat2x2<T>& operator*=(const Mat2x2<T>&);
		Mat2x2<T>& operator+=(const Mat2x2<T>&);
		Mat2x2<T>& operator-=(const Mat2x2<T>&);

		const T* getPtr() const;
		const BaseMatrix2x2<T>& getBase() const { return m_matrix; }

	private:
		BaseMatrix2x2<T> m_matrix; /// Depends on the platform
	};

	typedef Mat2x2<float> Mat2;

	template <typename T> Mat2x2<T> operator*(const Mat2x2<T>&,const Mat2x2<T>&);
	template <typename T> Mat2x2<T> operator+(const Mat2x2<T>&,const Mat2x2<T>&);
	template <typename T> Mat2x2<T> operator-(const Mat2x2<T>&,const Mat2x2<T>&);
}

// Platform-specific Implementation
#include "Math/Vectors/Mat4Implementation.h" 