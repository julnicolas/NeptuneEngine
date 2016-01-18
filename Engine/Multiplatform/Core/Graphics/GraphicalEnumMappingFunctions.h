#pragma once

#include "Graphics/IncludeOpenGL.h"
#include "Graphics/GraphicsProgram.h"
#include "Graphics/PLYLoader.h"
#include "Graphics/Renderer.h"

namespace Neptune
{
	GLenum MapDrawingPrimitive(Neptune::Renderer::DrawingPrimitive const p);
	GLenum MapType(const Neptune::GraphicsProgram::Types type);
	GraphicsProgram::Types MapType(const PLYLoader::ValueType type);
}