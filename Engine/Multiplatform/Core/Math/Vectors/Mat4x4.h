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

		/// \note Matrices are layed out using a column major order (OpenGL norm, 
		/// oposite order to what scientists and engineer are used to deal with)
		Mat4x4<T>(
			const T& _a1, const T& _b1, const T& _c1, const T& _d1,
			const T& _a2, const T& _b2, const T& _c2, const T& _d2,
			const T& _a3, const T& _b3, const T& _c3, const T& _d3,
			const T& _a4, const T& _b4, const T& _c4, const T& _d4
			) : m_matrix(
				_a1, _b1, _c1, _d1,
				_a2, _b2, _c2, _d2,
				_a3, _b3, _c3, _d3,
				_a4, _b4, _c4, _d4) {}


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
	template <typename T> bool		operator==(const Mat4x4<T>&, const Mat4x4<T>&);
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