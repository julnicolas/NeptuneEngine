#include "GL/glew.h"
#include "Graphics/GL_SDL2Context.h"
#include "SDL2/SDL.h"

// Must be reimplemented on each platform that uses SDL
namespace Neptune {
	namespace GL_SDL2{
		bool InitContext()
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
	}
}
