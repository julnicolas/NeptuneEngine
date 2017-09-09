#include "Graphics/View.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "Graphics/UniformVarNames.h"
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
		status = status && m_texture->update();
	}

	if ( m_camera != nullptr )
	{
		// Ouch dirtyyyyyyyy! mvp sent in mv uniform because the camera binding is messy! FIXME PLEASE
		Mat4 mvp = m_camera->getProjectionMatrix() * m_camera->getViewMatrix() * m_transform.getMatrix();
		
		updateUniform(NEP_UNIVNAME_MV_MATRIX,(void*)mvp.getPtr());
	}
	else 
	{
		updateUniform(NEP_UNIVNAME_MV_MATRIX,(void*)m_transform.getDataPtr());
	}

	status = status && m_renderer->update();
	return status;
}

bool View::updateUniform(GraphicsProgram::ProgramName _pgmName, const char* _uniformName, void* _value)
{
	auto        it  = m_programMap.find(_pgmName);
	
	NEP_ASSERT(it != m_programMap.end()); //  Error: Program doesn't exist

	return m_renderer->updateUniform(it->second, _uniformName, _value);
}

bool View::updateUniform(const char* _uniformName, void* _value)
{
	bool status = false;
	
	for (const auto& it : m_programMap)
	{
		Renderer::ProgramID id = it.second;

		status = status || m_renderer->updateUniform(id, _uniformName, _value);
	}
	
	return status;
}

void View::addGraphicsProgram(GraphicsProgram* _pgm)
{
	NEP_ASSERT(m_programMap.find(_pgm->getName()) == m_programMap.end()); // Error: two programs have the same name
	
	Renderer::ProgramID id        = m_renderer->addProgram( _pgm );
	m_programMap[_pgm->getName()] = id;
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