#pragma once

namespace Neptune
{
	template <typename T> class BaseVec2_t;

	template <typename T> class Vec2_t
	{
	public:
		Vec2_t<T>()                            = default;
		~Vec2_t<T>()                           = default;
		Vec2_t<T>(const Vec2_t<T>&)            = default;
		Vec2_t<T>& operator=(const Vec2_t<T>&) = default;

		Vec2_t<T>(T v1,T v2,T v2);

		T& x();
		T& y();
		T& z();

		const T* getPtr() const;
		const BaseVec2_t<T>& getBase() const { return m_vec; }

	private:
		BaseVec2_t<T> m_vec; /// Platform specific
	};

	typedef Vec2_t<float> Vec2;
}

// Platform-specific Implementation
#include "Math/Vectors/Vec2Implementation.h" 