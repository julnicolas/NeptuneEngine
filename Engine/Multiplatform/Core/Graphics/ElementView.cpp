#include "Graphics/ElementView.h"

using namespace Neptune;

ElementView::ElementView()
{
	m_renderer = new ElementRenderer;
}

ElementView::~ElementView()
{
	delete m_renderer;
}