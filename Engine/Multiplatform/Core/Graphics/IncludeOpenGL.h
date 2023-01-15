#pragma once

/// Handles every system-specific requirements to include OpenGL

#ifdef NEP_WINDOWS
	#include <GL/glew.h>
#else
	#include <GL/gl.h>
#endif