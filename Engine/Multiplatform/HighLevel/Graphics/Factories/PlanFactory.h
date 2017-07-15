#pragma once

#include "Graphics/ViewFactory.h"
#include "Graphics/VAOView.h"
#include "Graphics/Color.h"

namespace Neptune
{
	class PlanFactory: public ViewFactory
	{
	public:
		PlanFactory();
		PlanFactory(const Color& color);
		PlanFactory(Texture* texture);
		virtual ~PlanFactory()                     = default;
		PlanFactory(const PlanFactory&)            = default;
		PlanFactory& operator=(const PlanFactory&) = default;

		VAOView* create() override; /// Allocates a new VAOView on the heap. Class users must handle the object's deallocation.

	private:
		void initData(const Color& color);
		void initData(Texture* texture);
	};
}