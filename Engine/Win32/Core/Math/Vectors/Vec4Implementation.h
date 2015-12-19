#pragma once

#include <glm/vec4.hpp>

namespace Neptune
{
	template <typename T>
	class Vec4: public glm::tvec4<T>
	{
	public:
		Vec4<T>()                          = default;
		~Vec4<T>()                         = default;
		Vec4<T>(const Vec4<T>&)            = default;
		Vec4<T>& operator=(const Vec4<T>&) = default;

		Vec4<T>(T v1,T v2,T v3): glm::tvec4<T>(v1,v2,v3) {}
		const T* getPtr() const { return glm::value_ptr(*this); }
	};
}