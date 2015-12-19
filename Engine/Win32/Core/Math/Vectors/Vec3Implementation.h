#pragma once

#include <glm/vec3.hpp>

namespace Neptune
{
	template <typename T>
	class Vec3: public glm::tvec3<T>
	{
	public:
		Vec3<T>()                          = default;
		~Vec3<T>()                         = default;
		Vec3<T>(const Vec3<T>&)            = default;
		Vec3<T>& operator=(const Vec3<T>&) = default;

		Vec3<T>(T v1, T v2, T v3): glm::tvec3<T>(v1, v2, v3) {}
		const T* getPtr() const { return glm::value_ptr(*this); }
	};
}