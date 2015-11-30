#pragma once

#include "Graphics/View.h"

namespace Neptune
{
	class ElementView: public View
	{
	public:
		ElementView();
		virtual ~ElementView();
		ElementView(const ElementView&)            = delete;
		ElementView& operator=(const ElementView&) = delete;
	};
}