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

	case Renderer::QUADS:
		primitive = GL_QUADS;
		break;

	case Renderer::QUAD_STRIP:
		primitive = GL_QUAD_STRIP;
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

Neptune::GraphicsProgram::Types Neptune::MapType(const PLYLoader::ValueType type)
{
	GraphicsProgram::Types ret_type = GraphicsProgram::U8;

	switch(type)
	{
	case PLYLoader::UCHAR:
		ret_type = GraphicsProgram::U8;
		break;

	case PLYLoader::USHORT:
		NEP_ASSERT(false); // Not supported yet
		//ret_type = GraphicsProgram::U16;
		break;

	case PLYLoader::UINT:
		ret_type = GraphicsProgram::U32;
		break;

	case PLYLoader::FLOAT:
		ret_type = GraphicsProgram::FLOAT;
		break;

	case PLYLoader::DOUBLE:
		NEP_ASSERT(false); // Not supported yet
		//ret_type = GraphicsProgram::DOUBLE;
		break;

	default:
		NEP_ASSERT(false);
		break;
	}

	return ret_type;
}