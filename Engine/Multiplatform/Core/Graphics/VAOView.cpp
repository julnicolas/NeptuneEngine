#include "Graphics/VAOView.h"
#include "Graphics/VAORenderer.h"

using namespace Neptune;

VAOView::VAOView()
{
	m_renderer = new VAORenderer;
}

VAOView::~VAOView()
{
	delete m_renderer;
}