#pragma once

namespace Neptune
{
	template <typename T> class BaseVec4_t;

	template <typename T> class Vec4_t
	{
	public:
		Vec4_t<T>()                            = default;
		~Vec4_t<T>()                           = default;
		Vec4_t<T>(const Vec4_t<T>&)            = default;
		Vec4_t<T>& operator=(const Vec4_t<T>&) = default;

		Vec4_t<T>(T v1,T v2,T v3, T v4);

		T& x();
		T& y();
		T& z();
		T& w();

		const T* getPtr() const;
		const BaseVec4_t<T>& getBase() const { return m_vec; }

	private:
		BaseVec4_t<T> m_vec; /// Platform specific
	};

	typedef Vec4_t<float> Vec4;
}

// Platform-specific Implementation
#include "Math/Vectors/Vec4Implementation.h" 