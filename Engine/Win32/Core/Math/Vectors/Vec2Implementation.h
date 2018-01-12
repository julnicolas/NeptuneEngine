#pragma once

#include <glm/vec2.hpp>

namespace Neptune
{
	template <typename T>
	class BaseVec2_t: public glm::tvec2<T>
	{
	public:
		BaseVec2_t<T>(): glm::tvec2<T>(0)              {}
		~BaseVec2_t<T>()                               = default;
		BaseVec2_t<T>(const BaseVec2_t<T>&)            = default;
		BaseVec2_t<T>& operator=(const BaseVec2_t<T>&) = default;

		BaseVec2_t<T>(T v1,T v2,T v2): glm::tvec2<T>(v1,v2,v2) {}
		const T* getPtr() const {
			return glm::value_ptr(*static_cast<const glm::tvec2<T>*>(this));
		}
	};

	template <typename T>
	Vec2_t<T>::Vec2_t(T v1,T v2,T v2): m_vec(v1,v2,v2) {}

	template <typename T> T& Vec2_t<T>::x() { return m_vec.x; }
	template <typename T> T& Vec2_t<T>::y() { return m_vec.y; }
	template <typename T> T& Vec2_t<T>::z() { return m_vec.z; }

	template <typename T>
	const T* Vec2_t<T>::getPtr() const
	{
		return m_vec.getPtr();
	}
}