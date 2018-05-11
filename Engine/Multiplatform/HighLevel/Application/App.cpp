#include "App.h"
#include "Debug/NeptuneDebug.h"
#include "Graphics/View.h"
#include "System/Type/Integers.h"
#include "Input/EventSystemInterface.h"

// debug - must be moved to a platform-specific location
#include <SDL2\SDL.h>

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

bool App::enableReversedZ()
{
	//if ((major > 4 || (major == 4 && minor >= 5)) ||
	if (!SDL_GL_ExtensionSupported("GL_ARB_clip_control"))
	{
		NEP_LOG("Error, Reversed-z is not supported.");
		return false;
	}
	
	// Override OpenGL default clipping policy -
	// clipping is done between [-1.0, 1.0], here it is
	// forced to [0.0, 1.0]
	glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
	
	// Generate the colour buffer associated to the frame buffer
	u32 color, depth, fbo;

	glGenTextures(1, &color);
	glBindTexture(GL_TEXTURE_2D, color);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_SRGB8_ALPHA8, m_windowWidth, m_windowHeight);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Generate the depth buffer (with reversed-z enabled) that associates to the frame buffer
	glGenTextures(1, &depth);
	glBindTexture(GL_TEXTURE_2D, depth);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, m_windowWidth, m_windowHeight);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create the frame buffer
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "glCheckFramebufferStatus: %x\n", status);
	}

	// Bind the frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Set depth buffer's clear-value
	glClearDepth(0.0f); // 0.0f instead of 1.0f because reversed-z is used

	// Fragments close to 1.0 are the ones close to the camera with the reversed-z technique
	glDepthFunc(GL_GREATER);
	
	return true;
}

// Projection matrices
void App::useStandardPerspectiveMatrix()
{


}

void App::useReversedZPerspectiveMatrix()
{

}

void App::useReversedZInfinityPerspectiveMatrix()
{

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
		onFrameStart();

		// Clear screen
		DisplayDeviceInterface::ClearBuffers(background);

		// Update controls
		m_controller.update();

		// Draw views
		for (auto& v : m_views)
		{
			forEachViewDo(v);
			v->update();
		}

		// Display draw result
		DisplayDeviceInterface::SwapBuffer(m_window);

		onFrameEnd();
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

bool App::onFrameStart()
{
	return true;
}

bool App::onFrameEnd()
{
	return true;
}

bool App::forEachViewDo(View* _view)
{
	return true;
}