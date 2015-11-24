#include "Graphics/VAOView.h"

using namespace Neptune;

VAOView::VAOView()
{

}

VAOView::~VAOView()
{

}

bool VAOView::init()
{
	return m_renderer.init();
}

bool VAOView::update()
{
	return m_renderer.update();
}

void VAOView::terminate()
{
	m_renderer.terminate();
}