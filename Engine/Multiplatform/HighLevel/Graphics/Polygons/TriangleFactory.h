#pragma once

#include "Graphics/ViewFactory.h"
#include "Graphics/VAOView.h"

namespace Neptune
{
	class TriangleFactory : public ViewFactory
	{
	public:
		TriangleFactory();
		TriangleFactory(float r, float g, float b);
		virtual ~TriangleFactory()                         = default;
		TriangleFactory(const TriangleFactory&)            = default;
		TriangleFactory& operator=(const TriangleFactory&) = default;

		VAOView* create() override; /// Allocates a new VAOView on the heap. Class users must handle the object's deallocation.

	private:
		void initData(float r, float g, float b);
	};
}