#include "Graphics/View.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "Graphics/UniformVarNames.h"

#include "Debug/NeptuneDebug.h"

#include <algorithm>

using namespace Neptune;

bool View::init()
{
	return m_renderer->init();
}

bool View::cloneInit(View& _source)
{
	return m_renderer->cloneInit(*_source.m_renderer);
}

bool View::update()
{
	bool status = true;

	if ( m_camera != nullptr )
	{
		Mat4 MV         = m_camera->getViewMatrix() * m_transform.getMatrix();
		
		updateUniform(NEP_UNIVNAME_MV_MATRIX,(void*)MV.getPtr());
	}
	else 
		updateUniform(NEP_UNIVNAME_MV_MATRIX,(void*)m_transform.getDataPtr());

	status = status && m_renderer->update();
	return status;
}

void View::bindToCamera(Camera* c)
{
	NEP_ASSERT( c != nullptr );

	m_camera = c;
	updateUniform(NEP_UNIVNAME_PROJ_MATRIX,(void*)m_camera->getProjectionMatrix().getPtr());
}

void View::unbindFromCamera()
{
	m_camera = nullptr;

	Mat4 I;
	updateUniform(NEP_UNIVNAME_PROJ_MATRIX,(void*)I.getPtr());
}

bool View::updateUniform(GraphicsProgram::ProgramName _pgmName, const char* _uniformName, const void* _value)
{
	auto        it  = m_programMap.find(_pgmName);
	
	NEP_ASSERT(it != m_programMap.end()); //  Error: Program doesn't exist

	return m_renderer->updateUniform(it->second, _uniformName, _value);
}

bool View::updateUniform(const char* _uniformName, const void* _value)
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