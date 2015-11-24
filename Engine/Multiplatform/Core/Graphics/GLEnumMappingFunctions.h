#pragma once

#include "Graphics/IncludeOpenGL.h"
#include "Graphics/Renderer.h"

namespace Neptune
{
	GLenum MapDrawingPrimitive(Neptune::Renderer::DrawingPrimitive const p);
	GLenum MapType(const Neptune::GraphicalProgram::Types type);
}