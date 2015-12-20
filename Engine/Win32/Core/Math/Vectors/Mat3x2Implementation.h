#pragma once

#include <glm/mat3x2.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Neptune
{
	template <typename T>
	class BaseMatrix3x2: public glm::tmat3x2<T>
	{
	public:
		BaseMatrix3x2<T>(): glm::tmat3x2<T>(1)               {}
		~BaseMatrix3x2<T>()                                  = default;
		BaseMatrix3x2<T>(const BaseMatrix3x2<T>&)            = default;
		BaseMatrix3x2<T>& operator=(const BaseMatrix3x2<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(*(tmat3x2<T>*) this); }
	};


	////////////////////////////////////////////////////////////////////////////
	//
	//          I M P L E M E N TA T I O N   O F   M A T 3 x 2
	//
	///////////////////////////////////////////////////////////////////////////

	template <typename T>
	const T* Mat3x2<T>::getPtr() const
	{
		return m_matrix.getPtr();
	}

	template <typename T>
	Mat3x2<T>& Mat3x2<T>::operator*=(const Mat3x2<T>& m)
	{
		(glm::tmat3x2<T>&) m_matrix = m_matrix * m.m_matrix;
		return *this;
	}

	template <typename T>
	Mat3x2<T>& Mat3x2<T>::operator+=(const Mat3x2<T>& m)
	{
		(glm::tmat3x2<T>&) m_matrix = m_matrix + m.m_matrix;
		return *this;
	}

	template <typename T>
	Mat3x2<T>& Mat3x2<T>::operator-=(const Mat3x2<T>& m)
	{
		(glm::tmat3x2<T>&) m_matrix = m_matrix - m.m_matrix;
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////
	//
	//                 M A T 3 x 2 ' S   O P E R A T O R S
	//
	///////////////////////////////////////////////////////////////////////////

	template <typename T>
	Mat3x2<T> operator*(const Mat3x2<T>& ma,const Mat3x2<T>& mb)
	{
		using namespace glm;

		Mat3x2<T> r;
		(tmat3x2<T>&) r.getBase() = (tmat3x2<T>&) ma.getBase() * (tmat3x2<T>&) mb.getBase();

		return r;
	}

	template <typename T>
	Mat3x2<T> operator+(const Mat3x2<T>& ma,const Mat3x2<T>& mb)
	{
		using namespace glm;

		Mat3x2<T> r;
		(tmat3x2<T>&) r.getBase() = (tmat3x2<T>&) ma.getBase() + (tmat3x2<T>&) mb.getBase();

		return r;
	}

	template <typename T>
	Mat3x2<T> operator-(const Mat3x2<T>& ma,const Mat3x2<T>& mb)
	{
		using namespace glm;

		Mat3x2<T> r;
		(tmat3x2<T>&) r.getBase() = (tmat3x2<T>&) ma.getBase() - (tmat3x2<T>&) mb.getBase();

		return r;
	}
}