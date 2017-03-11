#pragma once

#include "Graphics/ViewFactory.h"
#include "Graphics/VAOView.h"

namespace Neptune
{
	class CubeFactory : public ViewFactory
	{
	public:
		CubeFactory();
		CubeFactory(const Color& color);
		virtual ~CubeFactory()                     = default;
		CubeFactory(const CubeFactory&)            = default;
		CubeFactory& operator=(const CubeFactory&) = default;

		VAOView* create() override; /// Allocates a new VAOView on the heap. Class users must handle the object's deallocation.

	private:
		static const u8 NB_VERTICES_TO_RENDER = 36;

		void initCubeData(const Color& color);
	};
}