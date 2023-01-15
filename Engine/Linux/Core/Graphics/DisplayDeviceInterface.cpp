#include "Graphics/GL_SDL2Context.h"
#include "Debug/NeptuneDebug.h"

namespace Neptune {
	namespace GL_SDL2 {
		bool InitContext() {
			NEP_LOG("WARNING - current InitContext is empty for Linux platforms");
			return true;
		}
	}
}