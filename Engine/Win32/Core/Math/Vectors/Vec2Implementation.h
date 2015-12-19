#pragma once

#include <glm/vec2.hpp>

namespace Neptune
{
	template <typename T>
	class Vec2_t: public glm::tvec2<T>
	{
	public:
		Vec2_t<T>()                          = default;
		~Vec2_t<T>()                         = default;
		Vec2_t<T>(const Vec2_t<T>&)            = default;
		Vec2_t<T>& operator=(const Vec2_t<T>&) = default;

		Vec2_t<T>(T v1,T v2,T v3): glm::tvec2<T>(v1,v2,v3) {}
		const T* getPtr() const { return glm::value_ptr(*this); }
	};
}