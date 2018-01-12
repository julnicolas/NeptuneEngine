#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Neptune
{
	template <typename T>
	class BaseMatrix4x4: public glm::tmat4x4<T>
	{
	public:
		BaseMatrix4x4<T>(): glm::tmat4x4<T>(1)               {}                          
		~BaseMatrix4x4<T>()                                  = default;
		BaseMatrix4x4<T>(const BaseMatrix4x4<T>&)            = default;
		BaseMatrix4x4<T>& operator=(const BaseMatrix4x4<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(* (tmat4x4<T>*) this); }
	};


	////////////////////////////////////////////////////////////////////////////
	//
	//          I M P L E M E N TA T I O N   O F   M A T 4 x 4
	//
	///////////////////////////////////////////////////////////////////////////

	template <typename T>
	const T* Mat4x4<T>::getPtr() const
	{
		return m_matrix.getPtr();
	}

	template <typename T>
	Mat4x4<T>& Mat4x4<T>::operator*=(const Mat4x4<T>& m)
	{
		(glm::tmat4x4<T>&) m_matrix = m_matrix * m.m_matrix;
		return *this;
	}

	template <typename T>
	Mat4x4<T>& Mat4x4<T>::operator+=(const Mat4x4<T>& m)
	{
		(glm::tmat4x4<T>&) m_matrix = m_matrix + m.m_matrix;
		return *this;
	}

	template <typename T>
	Mat4x4<T>& Mat4x4<T>::operator-=(const Mat4x4<T>& m)
	{
		(glm::tmat4x4<T>&) m_matrix = m_matrix - m.m_matrix;
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////
	//
	//                 M A T 4 x 4 ' S   O P E R A T O R S
	//
	///////////////////////////////////////////////////////////////////////////

	template <typename T>
	Mat4x4<T> operator*(const Mat4x4<T>& ma, const Mat4x4<T>& mb)
	{
		using namespace glm;

		Mat4x4<T> r;
		(tmat4x4<T>&) r.getBase() = (tmat4x4<T>&) ma.getBase() * (tmat4x4<T>&) mb.getBase();

		return r;
	}

	template <typename T>
	Mat4x4<T> operator+(const Mat4x4<T>& ma, const Mat4x4<T>& mb)
	{
		using namespace glm;

		Mat4x4<T> r;
		(tmat4x4<T>&) r.getBase() = (tmat4x4<T>&) ma.getBase() + (tmat4x4<T>&) mb.getBase();

		return r;
	}

	template <typename T>
	Mat4x4<T> operator-(const Mat4x4<T>& ma, const Mat4x4<T>& mb)
	{
		using namespace glm;

		Mat4x4<T> r;
		(tmat4x4<T>&) r.getBase() = (tmat4x4<T>&) ma.getBase() - (tmat4x4<T>&) mb.getBase();

		return r;
	}

	template <typename T>
	Vec4_t<T> operator*(const Mat4x4<T>& m, const Vec4_t<T>& v)
	{
		Vec4_t<T> r;
		(glm::tvec4<T>&) r.getBase() = (glm::tmat4x4<T>&) m.getBase() * (glm::tvec4<T>&) v.getBase();

		return r;
	}

	template <typename T> 
	Mat4x4<T> operator*(const Mat4x4<T>& m, T v)
	{
		using namespace glm;

		Mat4x4<T> r;
		(tmat4x4<T>&) r.getBase() = (tmat4x4<T>&) m.getBase() * v;

		return r;
	}

	template <typename T>
	Mat4x4<T> operator*(T v, const Mat4x4<T>& m)
	{
		using namespace glm;

		Mat4x4<T> r;
		(tmat4x4<T>&) r.getBase() = v * (tmat4x4<T>&) m.getBase();

		return r;
	}
}