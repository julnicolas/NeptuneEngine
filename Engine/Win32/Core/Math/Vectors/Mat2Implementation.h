#pragma once

#include <glm/mat2x2.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Neptune
{
	template <typename T>
	class BaseMatrix2x2: public glm::tmat2x2<T>
	{
	public:
		BaseMatrix2x2<T>(): glm::tmat2x2<T>(1)               {}
		~BaseMatrix2x2<T>()                                  = default;
		BaseMatrix2x2<T>(const BaseMatrix2x2<T>&)            = default;
		BaseMatrix2x2<T>& operator=(const BaseMatrix2x2<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(*(tmat2x2<T>*) this); }
	};


	////////////////////////////////////////////////////////////////////////////
	//
	//          I M P L E M E N TA T I O N   O F   M A T 2 x 2
	//
	///////////////////////////////////////////////////////////////////////////

	template <typename T>
	const T* Mat2x2<T>::getPtr() const
	{
		return m_matrix.getPtr();
	}

	template <typename T>
	Mat2x2<T>& Mat2x2<T>::operator*=(const Mat2x2<T>& m)
	{
		(glm::tmat2x2<T>&) m_matrix = m_matrix * m.m_matrix;
		return *this;
	}

	template <typename T>
	Mat2x2<T>& Mat2x2<T>::operator+=(const Mat2x2<T>& m)
	{
		(glm::tmat2x2<T>&) m_matrix = m_matrix + m.m_matrix;
		return *this;
	}

	template <typename T>
	Mat2x2<T>& Mat2x2<T>::operator-=(const Mat2x2<T>& m)
	{
		(glm::tmat2x2<T>&) m_matrix = m_matrix - m.m_matrix;
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////
	//
	//                 M A T 2 x 2 ' S   O P E R A T O R S
	//
	///////////////////////////////////////////////////////////////////////////

	template <typename T>
	Mat2x2<T> operator*(const Mat2x2<T>& ma,const Mat2x2<T>& mb)
	{
		using namespace glm;

		Mat2x2<T> r;
		(tmat2x2<T>&) r.getBase() = (tmat2x2<T>&) ma.getBase() * (tmat2x2<T>&) mb.getBase();

		return r;
	}

	template <typename T>
	Mat2x2<T> operator+(const Mat2x2<T>& ma,const Mat2x2<T>& mb)
	{
		using namespace glm;

		Mat2x2<T> r;
		(tmat2x2<T>&) r.getBase() = (tmat2x2<T>&) ma.getBase() + (tmat2x2<T>&) mb.getBase();

		return r;
	}

	template <typename T>
	Mat2x2<T> operator-(const Mat2x2<T>& ma,const Mat2x2<T>& mb)
	{
		using namespace glm;

		Mat2x2<T> r;
		(tmat2x2<T>&) r.getBase() = (tmat2x2<T>&) ma.getBase() - (tmat2x2<T>&) mb.getBase();

		return r;
	}
}