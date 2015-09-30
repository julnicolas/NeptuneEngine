#include "Graphics/GraphicalContext.h"
#include "Debug/NeptuneDebug.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>

using namespace Neptune;


bool GraphicalContext::CreateGraphicalContext(const char* windowName, u16 width,u16 height,u8 minCtxtVersion,u8 maxCtxtVersion)
{
	const u8 Z_BUFFER_LENGTH = 32; // Platform specific
	
	// Initialize the SDL's video system
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		NEP_LOG("ERROR: Couldn't Initialize video system\n");
		return false;
	}

	// Set the desired context version
	SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 ); // Guarantees hardware acceleration
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, maxCtxtVersion );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, minCtxtVersion );

	// Set up buffer sizes
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, Z_BUFFER_LENGTH );

	// Enable double buffering
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	// Create the main window
	SDL_Window* window = SDL_CreateWindow(	windowName, 
											SDL_WINDOWPOS_CENTERED,
											SDL_WINDOWPOS_CENTERED,
											width,
											height,
											SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

	// Did window creation go wrong?
	if ( !window )
	{
		NEP_LOG("ERROR: Couldn't create main window");
		SDL_Quit();
		return false;
	}

	// Create an OpenGL context and attach it to the window
	SDL_GLContext context = SDL_GL_CreateContext( window );
	
	if ( !InitContext() ) // Platform specific
		return false;

	// Synchronize buffer swapping with the screen's refresh rate
	SDL_GL_SetSwapInterval(1); // Use VSync
}

void GraphicalContext::DestructGraphicalContext()
{
	SDL_GL_DeleteContext();
	SDL_DestroyWindow();
}

// Must be implemented on each platform that uses SDL
static bool InitContext()
{
	if ( glewInit() != GLEW_OK )
	{
		NEP_LOG("Error: Graphical Context couldn't be initialized");
		SDL_Quit();
		return false;
	}

	return true;
}