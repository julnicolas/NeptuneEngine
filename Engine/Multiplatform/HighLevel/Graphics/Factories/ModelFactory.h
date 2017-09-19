#pragma once

#include "Graphics/Factories/ViewFactory.h"
#include "Graphics/ElementView.h"
#include "Graphics/PLYLoader.h" // Includes indirectly std::string

namespace Neptune
{
	class ModelFactory final: public ViewFactory
	{
	public:
		ModelFactory(const char* fileName);
		virtual ~ModelFactory()                      = default;
		ModelFactory(const ModelFactory&)            = default;
		ModelFactory& operator=(const ModelFactory&) = default;

		ElementView* create() final override; /// Allocates a new VAOView on the heap. Class users must handle the object's deallocation.

	private:
		PLYLoader   m_loader; /// A specific format is used here because only one file format is currently supported

#ifdef NEP_DEBUG
		std::string m_dbg_fileName;
#endif
	};

}