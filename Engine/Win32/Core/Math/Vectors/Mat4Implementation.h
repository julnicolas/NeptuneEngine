#pragma once

#include <glm/mat4x4.hpp>

namespace Neptune
{
	template <typename T>
	class Mat4: public glm::tmat4x4<T>
	{
	public:
		Mat4<T>()                          = default;
		~Mat4<T>()                         = default;
		Mat4<T>(const Mat4<T>&)            = default;
		Mat4<T>& operator=(const Mat4<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(*this); }
	};
}