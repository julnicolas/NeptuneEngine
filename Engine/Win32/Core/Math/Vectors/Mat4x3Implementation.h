#pragma once

#include <glm/mat4x3.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Neptune
{
	template <typename T>
	class BaseMatrix4x3: public glm::tmat4x3<T>
	{
	public:
		BaseMatrix4x3<T>(): glm::tmat4x3<T>(1)               {}
		~BaseMatrix4x3<T>()                                  = default;
		BaseMatrix4x3<T>(const BaseMatrix4x3<T>&)            = default;
		BaseMatrix4x3<T>& operator=(const BaseMatrix4x3<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(*(tmat4x3<T>*) this); }
	};


	////////////////////////////////////////////////////////////////////////////
	//
	//          I M P L E M E N TA T I O N   O F   M A T 4 x 3
	//
	///////////////////////////////////////////////////////////////////////////

	template <typename T>
	const T* Mat4x3<T>::getPtr() const
	{
		return m_matrix.getPtr();
	}

	template <typename T>
	Mat4x3<T>& Mat4x3<T>::operator*=(const Mat4x3<T>& m)
	{
		(glm::tmat4x3<T>&) m_matrix = m_matrix * m.m_matrix;
		return *this;
	}

	template <typename T>
	Mat4x3<T>& Mat4x3<T>::operator+=(const Mat4x3<T>& m)
	{
		(glm::tmat4x3<T>&) m_matrix = m_matrix + m.m_matrix;
		return *this;
	}

	template <typename T>
	Mat4x3<T>& Mat4x3<T>::operator-=(const Mat4x3<T>& m)
	{
		(glm::tmat4x3<T>&) m_matrix = m_matrix - m.m_matrix;
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////
	//
	//                 M A T 4 x 3 ' S   O P E R A T O R S
	//
	///////////////////////////////////////////////////////////////////////////

	template <typename T>
	Mat4x3<T> operator*(const Mat4x3<T>& ma,const Mat4x3<T>& mb)
	{
		using namespace glm;

		Mat4x3<T> r;
		(tmat4x3<T>&) r.getBase() = (tmat4x3<T>&) ma.getBase() * (tmat4x3<T>&) mb.getBase();

		return r;
	}

	template <typename T>
	Mat4x3<T> operator+(const Mat4x3<T>& ma,const Mat4x3<T>& mb)
	{
		using namespace glm;

		Mat4x3<T> r;
		(tmat4x3<T>&) r.getBase() = (tmat4x3<T>&) ma.getBase() + (tmat4x3<T>&) mb.getBase();

		return r;
	}

	template <typename T>
	Mat4x3<T> operator-(const Mat4x3<T>& ma,const Mat4x3<T>& mb)
	{
		using namespace glm;

		Mat4x3<T> r;
		(tmat4x3<T>&) r.getBase() = (tmat4x3<T>&) ma.getBase() - (tmat4x3<T>&) mb.getBase();

		return r;
	}
}