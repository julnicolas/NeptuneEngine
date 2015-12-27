#pragma once

#include "Graphics/ViewFactory.h"
#include "Graphics/ElementView.h"
#include "Graphics/PLYLoader.h"

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

		PLYLoader   m_loader; /// A specific format is used here because only one file format is currently supported
		std::string m_fileName;
	};

}