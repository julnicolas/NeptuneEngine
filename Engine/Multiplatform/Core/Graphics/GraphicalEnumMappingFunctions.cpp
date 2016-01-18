#include "Graphics/GraphicalEnumMappingFunctions.h"
#include "Debug/NeptuneDebug.h"

GLenum Neptune::MapDrawingPrimitive(Renderer::DrawingPrimitive const p)
{
	GLenum primitive = GL_INVALID_ENUM;

	switch(p)
	{
	case Renderer::TRIANGLES:
		primitive = GL_TRIANGLES;
		break;

	case Renderer::TRIANGLE_STRIP:
		primitive = GL_TRIANGLE_STRIP;
		break;

	case Renderer::TRIANGLE_FAN:
		primitive = GL_TRIANGLE_FAN;
		break;

	default:
		NEP_ASSERT(false);
		break;
	}

	return primitive;
}

GLenum Neptune::MapType(const GraphicsProgram::Types type)
{
	GLenum gl_type = GL_INVALID_ENUM;

	switch(type)
	{
	case GraphicsProgram::Types::U8:
		gl_type = GL_UNSIGNED_BYTE;
		break;

	case GraphicsProgram::Types::U32:
		gl_type = GL_UNSIGNED_INT;
		break;

	case GraphicsProgram::Types::S32:
		gl_type = GL_INT;
		break;

	case GraphicsProgram::Types::FLOAT:
		gl_type = GL_FLOAT;
		break;

	default:
		NEP_ASSERT(false);
		break;
	}

	return gl_type;
}