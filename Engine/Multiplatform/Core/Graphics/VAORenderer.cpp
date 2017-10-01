#include "Graphics/VAORenderer.h"
#include "Graphics/IncludeOpenGL.h"
#include "Debug/NeptuneDebug.h"
#include "Graphics/GraphicalEnumMappingFunctions.h"

using namespace Neptune;

const u32 INVALID_VAO_HANDLE = ~0;

VAORenderer::VAORenderer():
m_vao(INVALID_VAO_HANDLE)
{

}

bool VAORenderer::init()
{
	NEP_ASSERT(m_vao == INVALID_VAO_HANDLE); // Renderer hasn't been initialized
	
	// Generate VAO
	glGenVertexArrays( 1, &m_vao );
	
	// Bind the VAO 
	glBindVertexArray( m_vao );
	NEP_GRAPHICS_ASSERT();

	// Execute parent's renderer
	bool status = Renderer::init();

	NEP_ASSERT( status );
	return status;
}

void VAORenderer::terminate()
{
	glDeleteVertexArrays( 1, &m_vao );
	Renderer::terminate();
}

void VAORenderer::draw()
{
	glDrawArrays( MapDrawingPrimitive( m_drawingPrimitive ) , 0, m_nbverticesToRender );
	NEP_GRAPHICS_ASSERT();
}

void VAORenderer::bindShaderAttributes(const GraphicsProgram& pgm)
{
	glBindVertexArray( m_vao );
	Renderer::bindShaderAttributes( pgm );
}