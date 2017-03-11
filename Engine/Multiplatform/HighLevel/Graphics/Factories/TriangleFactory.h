#pragma once

#include "Graphics/ViewFactory.h"
#include "Graphics/VAOView.h"
#include "Graphics/Color.h"

namespace Neptune
{
	class TriangleFactory : public ViewFactory
	{
	public:
		TriangleFactory();
		TriangleFactory(const Color& color);
		virtual ~TriangleFactory()                         = default;
		TriangleFactory(const TriangleFactory&)            = default;
		TriangleFactory& operator=(const TriangleFactory&) = default;

		VAOView* create() override; /// Allocates a new VAOView on the heap. Class users must handle the object's deallocation.

	private:
		void initData(const Color& color);
	};
}