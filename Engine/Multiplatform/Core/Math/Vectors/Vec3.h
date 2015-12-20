#pragma once

namespace Neptune
{
	template <typename T> class BaseVec3_t;

	template <typename T> class Vec3_t
	{
	public:
		Vec3_t<T>()                            = default;
		~Vec3_t<T>()                           = default;
		Vec3_t<T>(const Vec3_t<T>&)            = default;
		Vec3_t<T>& operator=(const Vec3_t<T>&) = default;

		Vec3_t<T>(T v1,T v2,T v3);

		T& x();
		T& y();
		T& z();

		const T* getPtr() const;
		const BaseVec3_t<T>& getBase() const { return m_vec; }

	private:
		BaseVec3_t<T> m_vec; /// Platform specific
	};

	typedef Vec3_t<float> Vec3;
}

// Platform-specific Implementation
#include "Math/Vectors/Vec3Implementation.h" 