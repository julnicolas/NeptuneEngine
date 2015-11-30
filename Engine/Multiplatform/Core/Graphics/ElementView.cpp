#include "Graphics/ElementView.h"
#include "Graphics/ElementRenderer.h"

using namespace Neptune;

ElementView::ElementView()
{
	m_renderer = new ElementRenderer;
}

ElementView::~ElementView()
{
	delete m_renderer;
}