#include "Graphics/DisplayDeviceInterface.h"
#include "Debug/NeptuneDebug.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>

using namespace Neptune;

// Must be reimplemented on each platform that uses SDL
static bool InitContext()
{
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		NEP_LOG("Error: Graphical Context couldn't be initialized");
		SDL_Quit();
		return false;
	}

	return true;
}

DisplayDeviceInterface::WindowHandle DisplayDeviceInterface::CreateWindow(const char* name, u32 width, u32 height, bool fullScreen /*= false*/)
{
	const u8 OGL_MAJOR_VERSION   = 4;
	const u8 OGL_MINOR_VERSION   = 3;

	// The compatibility profile must be used on Windows because of a bug in Glew
	// that causes invalid GL_ENUM_ERRORS to be raised.
	const u8 OGL_CONTEXT_PROFILE = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
	
	// Initialize the SDL's video system
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		NEP_LOG("ERROR: Couldn't Initialize video system\n");
		return nullptr;
	}

	// Prior window-creation context-initialization
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,  OGL_CONTEXT_PROFILE ); // Selects the OpenGl profile to use
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OGL_MAJOR_VERSION);    // Wished work-version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OGL_MINOR_VERSION);    // Min supported version

	// Create the main window
	SDL_Window* window = nullptr;
	if ( !fullScreen )
	{
		window = SDL_CreateWindow(name,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	}
	else
	{
		window = SDL_CreateWindow(name,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
	}

	// Did window creation go wrong?
	if(!window)
	{
		NEP_LOG("ERROR: Couldn't create main window");
		SDL_Quit();
		return nullptr;
	}

	return static_cast<WindowHandle>( window );
}

void DisplayDeviceInterface::DestroyWindow(WindowHandle handle)
{
	SDL_Window* window = static_cast<SDL_Window*>( handle );
	SDL_DestroyWindow( window );
}

DisplayDeviceInterface::GraphicalContextHandle DisplayDeviceInterface::CreateGraphicalContext(WindowHandle window, u8 minCtxtVersion,u8 maxCtxtVersion)
{
	// Platform specifics
	const u8 Z_BUFFER_LENGTH = 32;
	SDL_Window* win = static_cast<SDL_Window*>( window );

	// Enable hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,1);

	// Set up buffer sizes
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,Z_BUFFER_LENGTH);
	
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

	// Create an OpenGL context and attach it to the window
	SDL_GLContext context = SDL_GL_CreateContext( win );

	if(!InitContext()) // Platform specific
		return nullptr;

	// Synchronize buffer swapping with the screen's refresh rate
	SDL_GL_SetSwapInterval(1); // Use VSync

	// Set up basic rendering settings
	glEnable(GL_DEPTH_TEST); // Enables depth test
	glDepthFunc(GL_LESS);    // Accepts fragment if it closer to the camera than the former one

	return static_cast<GraphicalContextHandle*>( context );
}

void DisplayDeviceInterface::DestroyGraphicalContext(GraphicalContextHandle handle)
{
	SDL_GLContext* context = static_cast<SDL_GLContext*>( handle );
	SDL_GL_DeleteContext( context );
}

void DisplayDeviceInterface::ClearBuffers(float backGroundColor[4])
{
	glClearBufferfv( GL_COLOR, 0, backGroundColor );
	glClear( GL_DEPTH_BUFFER_BIT );
}

void DisplayDeviceInterface::SwapBuffer(WindowHandle handle)
{
	SDL_Window* window = static_cast<SDL_Window*>( handle );
	SDL_GL_SwapWindow( window );
}