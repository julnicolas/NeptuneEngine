#pragma once

#include <glm/vec2.hpp>

namespace Neptune
{
	template <typename T>
	class Vec2: public glm::tvec2<T>
	{
	public:
		Vec2<T>()                          = default;
		~Vec2<T>()                         = default;
		Vec2<T>(const Vec2<T>&)            = default;
		Vec2<T>& operator=(const Vec2<T>&) = default;

		Vec2<T>(T v1,T v2,T v3): glm::tvec2<T>(v1,v2,v3) {}
		const T* getPtr() const { return glm::value_ptr(*this); }
	};
}