#pragma once

#include <glm/mat4x3.hpp>

namespace Neptune
{
	template <typename T>
	class Mat4x3: public glm::tmat4x3<T>
	{
		Mat4x3<T>()                            = default;
		~Mat4x3<T>()                           = default;
		Mat4x3<T>(const Mat4x3<T>&)            = default;
		Mat4x3<T>& operator=(const Mat4x3<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(*this); }
	};
}