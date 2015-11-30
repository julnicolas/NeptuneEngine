#include "Graphics/View.h"
#include "Graphics/Renderer.h"

using namespace Neptune;

bool View::init()
{
	return m_renderer->init();
}

bool View::update()
{
	bool v = m_renderer->updateUniform(0,"ModelView",(void*)m_transform.getDataPtr());
	return m_renderer->update();
}

void View::terminate()
{
	m_renderer->terminate();
}