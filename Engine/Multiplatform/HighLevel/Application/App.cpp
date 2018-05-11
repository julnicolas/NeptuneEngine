#include "App.h"
#include "Debug/NeptuneDebug.h"
#include "Graphics/View.h"
#include "System/Type/Integers.h"
#include "Input/EventSystemInterface.h"

using namespace Neptune;

App::App(const char* _appName):
	App::App()
{
	// Delete previous name
	delete[] m_appName;

	// Create new one from input and store it
	char* app_name = new char[strlen(_appName)];
	strcpy(app_name, _appName);
	m_appName = app_name;
}

App::App():
	m_appName(nullptr)
{
	// Sets viewport's length and width
	const u32 DEFAULT_WIDTH = 1024, DEFAULT_HEIGHT = 768;

	m_windowHeight = DEFAULT_HEIGHT;
	m_windowWidth  = DEFAULT_WIDTH;

	// Creates a FPS view
	m_controller.bindCamera(&m_camera);

	// Set app's name
	const u8 DEFAULT_NAME_LENGTH = 20;
	char* default_app_name = new char[DEFAULT_NAME_LENGTH];
	strcpy_s(default_app_name, DEFAULT_NAME_LENGTH, "Neptune Application");
	m_appName = default_app_name;
}

bool App::init()
{
	m_window  = DisplayDeviceInterface::CreateWindow(m_appName, m_windowWidth, m_windowHeight);
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

App::~App()
{
	delete[] m_appName;
}