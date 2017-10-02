#include "Graphics/ElementRenderer.h"
#include "Graphics/IncludeOpenGL.h"
#include "Debug/NeptuneDebug.h"
#include "Graphics/GraphicalEnumMappingFunctions.h"

using namespace Neptune;

const u32 INVALID_INDEX_BUFFER_HANDLE = ~0;

ElementRenderer::ElementRenderer():
	m_indexBufferID(INVALID_INDEX_BUFFER_HANDLE),
	m_indexType(IndexType::U8),
	m_indexBufferSize(0),
	m_indexBuffer(nullptr)
{

}

bool ElementRenderer::init()
{
	// Init check
	NEP_ASSERT( m_indexBufferID == INVALID_INDEX_BUFFER_HANDLE );	// Error Renderer has already been initialized
	NEP_ASSERT( m_indexBufferSize != 0 );							// Error setIndexBufferData() hasn't been called
	
	// Populate the index buffer
	glGenBuffers( 1, &m_indexBufferID );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferSize, m_indexBuffer, GL_STATIC_DRAW);
	
	// Execute parent's renderer
	bool status = Renderer::init();

	NEP_ASSERT( status );
	return status;
}

bool Neptune::ElementRenderer::cloneInit(const Renderer& _source)
{
	// Type check
	const ElementRenderer* source = dynamic_cast<const ElementRenderer*>(&_source);
	NEP_ASSERT( source != nullptr ); // Invalid argument, _source must be an ElementRenderer

	// Init check
	NEP_ASSERT( source->m_indexBufferID != INVALID_INDEX_BUFFER_HANDLE ); // Error _source hasn't been initialized
	NEP_ASSERT( m_indexBufferID			== INVALID_INDEX_BUFFER_HANDLE ); // Error Renderer has already been initialized

	// Copy object
	*this = *source;

	// Call parent
	return Renderer::cloneInit(*source);
}

void ElementRenderer::terminate()
{
	glDeleteBuffers( 1, &m_indexBufferID );
	m_indexBufferID = INVALID_INDEX_BUFFER_HANDLE; // Necessary so that init can be called again

	Renderer::terminate();
}

void ElementRenderer::setIndexBufferData(const void* data, u32 dataSize, ElementRenderer::IndexType dataType)
{
	m_indexType			= dataType;
	m_indexBufferSize	= dataSize;
	m_indexBuffer		= data;
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);

	// Bind the attributes
	Renderer::bindShaderAttributes( pgm );
}