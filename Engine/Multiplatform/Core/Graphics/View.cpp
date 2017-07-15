#include "Graphics/View.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "Graphics/UniformVarNames.h"
#include "Graphics/AmbientLight.h"
#include "Graphics/Texture.h"

#include "Debug/NeptuneDebug.h"

using namespace Neptune;

bool View::init()
{
	return m_renderer->init();
}

bool View::update()
{
	bool status = true;
	
	if(m_texture != nullptr)
	{
		NEP_ASSERT(m_texture->isInitialized());
		status = status && m_texture->update();
	}

	if ( m_camera != nullptr )
	{
		// Ouch dirtyyyyyyyy! mvp sent in mv uniform because the camera binding is messy! FIXME PLEASE
		Mat4 mvp = m_camera->getProjectionMatrix() * m_camera->getViewMatrix() * m_transform.getMatrix();
		
		m_renderer->updateUniform(0,NEP_UNIVNAME_MV_MATRIX,(void*)mvp.getPtr());
	}
	else 
	{
		m_renderer->updateUniform(0,NEP_UNIVNAME_MV_MATRIX,(void*)m_transform.getDataPtr());
	}

	// UpdateAmbientLight()
	if(m_ambientLight != nullptr)
	{
		const void* color = static_cast<const void*>(&(m_ambientLight->getColor()));
		m_renderer->updateUniform(0,NEP_UNIVNAME_AMBIENT_LIGHT,color); // Dirty because the light's intensity is ignored
	}


	status = status && m_renderer->update();
	return status;
}

void View::terminate()
{
	m_renderer->terminate();
}

void* View::operator new(size_t count)
{
	void* ptr = _aligned_malloc(count,16);
	return ptr;
}

void View::operator delete(void* ptr)
{
	_aligned_free(ptr);
}