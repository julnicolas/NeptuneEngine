#include "App.h"
#include "Debug/NeptuneDebug.h"
#include <algorithm>

using namespace Neptune;

std::list<InputSensitiveController*>  App::m_controllers;

App::App():
	m_inputEvent(nullptr)
{
	
}

void App::execute()
{
	executeControllers();
	m_renderer.draw();
}

InputSensitiveController::ControllerId App::registerController(InputSensitiveController* const ctrl)
{
	size_t pos = m_controllers.size();
	NEP_ASSERT( pos < (1 << sizeof(InputSensitiveController::ControllerId)*8) - 1 ); // pos doesn't exceed the max codable value?
	m_controllers.push_back(ctrl);

	return pos;
}

void App::unregisterController(InputSensitiveController* const ctrl)
{
	std::list<InputSensitiveController*>::iterator it = std::find( m_controllers.begin(), m_controllers.end(), ctrl );
	NEP_ASSERT( it != m_controllers.end() );
	
	m_controllers.erase(it);
}

void App::executeControllers()
{
	if ( fetchAndCopyFirstInputEvent() )
	{
		std::list<InputSensitiveController*>::iterator it_end = m_controllers.end();
		for ( std::list<InputSensitiveController*>::iterator it = m_controllers.begin(); it != it_end; ++it )
			(*it)->execute(m_inputEvent);
	}
}