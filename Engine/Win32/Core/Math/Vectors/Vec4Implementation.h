#pragma once

#include <glm/vec4.hpp>

namespace Neptune
{
	template <typename T>
	class Vec4_t: public glm::tvec4<T>
	{
	public:
		Vec4_t<T>()                          = default;
		~Vec4_t<T>()                         = default;
		Vec4_t<T>(const Vec4_t<T>&)            = default;
		Vec4_t<T>& operator=(const Vec4_t<T>&) = default;

		Vec4_t<T>(T v1,T v2,T v3): glm::tvec4<T>(v1,v2,v3) {}
		const T* getPtr() const { return glm::value_ptr(*this); }
	};
}