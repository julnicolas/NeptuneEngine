#pragma once

#include <glm/vec3.hpp>

namespace Neptune
{
	template <typename T>
	class Vec3_t: public glm::tvec3<T>
	{
	public:
		Vec3_t<T>()                            = default;
		~Vec3_t<T>()                           = default;
		Vec3_t<T>(const Vec3_t<T>&)            = default;
		Vec3_t<T>& operator=(const Vec3_t<T>&) = default;

		Vec3_t<T>(T v1, T v2, T v3): glm::tvec3<T>(v1, v2, v3) {}
		const T* getPtr() const { return glm::value_ptr(*this); }
	};
}