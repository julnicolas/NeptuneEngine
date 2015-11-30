#pragma once

#include "Graphics/View.h"

namespace Neptune
{
	class VAOView : public View
	{
	public:
		VAOView();
		virtual ~VAOView();
		VAOView(const VAOView&)            = delete;
		VAOView& operator=(const VAOView&) = delete;
	};
}