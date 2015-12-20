#include "Graphics/View.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"

using namespace Neptune;

bool View::init()
{
	return m_renderer->init();
}

bool View::update()
{
	if ( m_camera != nullptr )
	{
		Mat4 mvp = m_camera->getProjectionMatrix() * m_camera->getViewMatrix() * m_transform.getMatrix();
		
		m_renderer->updateUniform(0,"ModelView",(void*) mvp.getPtr());
		return m_renderer->update();
	}
	else 
	{
		m_renderer->updateUniform(0,"ModelView",(void*)m_transform.getDataPtr());
		return m_renderer->update();
	}
}

void View::terminate()
{
	m_renderer->terminate();
}