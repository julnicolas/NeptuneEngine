#include "Graphics/ElementRenderer.h"
#include "Graphics/IncludeOpenGL.h"
#include "Debug/NeptuneDebug.h"
#include "Graphics/GraphicalEnumMappingFunctions.h"

using namespace Neptune;

const u32 INVALID_INDEX_BUFFER_HANDLE = ~0;

bool ElementRenderer::init()
{
	NEP_ASSERT( m_indexBuffer == INVALID_INDEX_BUFFER_HANDLE ); // Renderer hasn't been initialized
	
	// Generate the index buffer
	glGenBuffers( 1, &m_indexBuffer );
	
	// Execute parent's renderer
	bool status = Renderer::init();

	NEP_ASSERT( status );
	return status;
}

void ElementRenderer::terminate()
{
	glDeleteBuffers( 1, &m_indexBuffer );
	Renderer::terminate();
}

void ElementRenderer::setIndexBufferData(const void* data, u32 dataSize, ElementRenderer::IndexType dataType)
{
	m_indexType = dataType;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
}

static GLenum MapType(const ElementRenderer::IndexType type)
{
	GLenum gl_type = GL_INVALID_ENUM;

	switch(type)
	{
	case ElementRenderer::IndexType::U8:
		gl_type = GL_UNSIGNED_BYTE;
		break;

	case ElementRenderer::IndexType::U16:
		gl_type = GL_UNSIGNED_SHORT;
		break;

	case ElementRenderer::IndexType::U32:
		gl_type = GL_UNSIGNED_INT;
		break;

	default:
		NEP_ASSERT(false);
		break;
	}

	return gl_type;
}

void ElementRenderer::draw()
{
	glDrawElements( MapDrawingPrimitive(m_drawingPrimitive), m_nbverticesToRender, ::MapType(m_indexType), 0 );
	NEP_GRAPHICS_ASSERT();
}

void ElementRenderer::bindShaderAttributes(const GraphicsProgram& pgm)
{
	// Bind the indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	// Bind the attributes
	Renderer::bindShaderAttributes( pgm );
}
