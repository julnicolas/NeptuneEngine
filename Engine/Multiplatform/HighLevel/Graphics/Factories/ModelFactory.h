#pragma once

#include "Graphics/ViewFactory.h"
#include "Graphics/ElementView.h"

namespace Neptune
{
	class ModelFactory : public ViewFactory
	{
	public:
		ModelFactory(const char* fileName);
		virtual ~ModelFactory()                      = default;
		ModelFactory(const ModelFactory&)            = default;
		ModelFactory& operator=(const ModelFactory&) = default;

		ElementView* create() override; /// Allocates a new VAOView on the heap. Class users must handle the object's deallocation.

	private:
		void initModelData(const char* fileName);

		std::string m_fileName;
	};

}