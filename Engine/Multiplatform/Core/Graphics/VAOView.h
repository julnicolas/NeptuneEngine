#pragma once

#include "Graphics/View.h"
#include "Graphics/VAORenderer.h"

namespace Neptune
{
	class VAOView : public View
	{
	public:
		VAOView();
		virtual ~VAOView();
		VAOView(const VAOView&)            = delete;
		VAOView& operator=(const VAOView&) = delete;

		bool init()             override;
		bool update()           override;
		void terminate()        override;
		Renderer& getRenderer() override { return m_renderer; }

	private:
		VAORenderer m_renderer;
	};
}