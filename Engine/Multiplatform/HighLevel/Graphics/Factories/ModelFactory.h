#pragma once

#include "Graphics/ViewFactory.h"
#include "Graphics/ElementView.h"

namespace Neptune
{
	class ModelFactory : public ViewFactory
	{
	public:
		ModelFactory();
		ModelFactory(float r,float g,float b);
		virtual ~ModelFactory()                      = default;
		ModelFactory(const ModelFactory&)            = default;
		ModelFactory& operator=(const ModelFactory&) = default;

		ElementView* create() override; /// Allocates a new VAOView on the heap. Class users must handle the object's deallocation.

	private:
		void initCubeData(float r,float g,float b);
	};

}