#pragma once

#include "Graphics/View.h"
#include "Graphics/ElementRenderer.h"

namespace Neptune
{
	class ElementView: public View
	{
	public:
		ElementView();
		virtual ~ElementView();
		ElementView(const ElementView&)            = delete;
		ElementView& operator=(const ElementView&) = delete;

		void setIndexBufferData(const void* _data, u32 _dataSize, ElementRenderer::IndexType _dataType) {
			 // The renderer MUST be an ElementRenderer, otherwise the class implementation is wrong.
			static_cast<ElementRenderer*>( m_renderer )->setIndexBufferData(_data, _dataSize, _dataType);
		}
	};
}