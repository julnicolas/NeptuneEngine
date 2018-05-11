#include "App.h"
#include "Debug/NeptuneDebug.h"
#include "Graphics/View.h"
#include "System/Type/Integers.h"
#include "Input/EventSystemInterface.h"

using namespace Neptune;

App::App()
{
	const u32 DEFAULT_WIDTH = 1024, DEFAULT_HEIGHT = 768;

	m_windowHeight = DEFAULT_HEIGHT;
	m_windowWidth  = DEFAULT_WIDTH;
	m_controller.bindCamera(&m_camera);
}

bool App::init()
{
	m_window  = DisplayDeviceInterface::CreateWindow("MultiTexturedModelExample", m_windowWidth, m_windowHeight);
	m_context = DisplayDeviceInterface::CreateGraphicalContext(m_window, 3, 4);
	EventSystemInterface::StartUp();
	m_controller.init();

	return true;
}


bool App::update()
{
	float background[4] = {0.0f, 0.0f, 0.0f, 0.0f}; // opaque black
	
	bool is_running = true;
	while (is_running)
	{
		// Clear screen
		DisplayDeviceInterface::ClearBuffers(background);

		// Update controls
		m_controller.update();

		// TODO : Add user-function here (template method)
		// should return true value for the main loop to continue, false otherwise.

		// Draw views
		for (auto& v : m_views)
		{
			v->update();
		}

		// Display draw result
		DisplayDeviceInterface::SwapBuffer(m_window);
	}

	return true;
}

void App::terminate()
{
	// Delete views
	for (auto& v : m_views){
		v->terminate();
		delete v;
	}
	m_views.clear();

	// Delete viewport
	DisplayDeviceInterface::DestroyWindow(m_window);
	DisplayDeviceInterface::DestroyGraphicalContext(m_context);
	
	// Delete control manager
	EventSystemInterface::ShutDown();
}