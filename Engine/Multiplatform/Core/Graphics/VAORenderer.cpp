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
	// Init check
	NEP_ASSERT(m_vao == INVALID_VAO_HANDLE); // Error Renderer has already been initialized
	
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

bool Neptune::VAORenderer::cloneInit(const Renderer& _source)
{
	// Init check
	NEP_ASSERT(m_vao == INVALID_VAO_HANDLE); // Error Renderer has already been initialized
	
	// Type check
	const VAORenderer* source = dynamic_cast<const VAORenderer*>(&_source);
	NEP_ASSERT( source != nullptr ); // Invalid argument, _source must be a VAORenderer

	// Copy object
	*this = *source;

	// Call parent
	return Renderer::cloneInit(*source);
}

void VAORenderer::terminate()
{
	glDeleteVertexArrays( 1, &m_vao );
	m_vao = INVALID_VAO_HANDLE; // Necessary so that init can be called again

	Renderer::terminate();
}

void VAORenderer::draw()
{
	const u32 WIDTH = 1024, HEIGHT = 768;

	glBindFramebuffer(GL_FRAMEBUFFER, 1);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 1, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 2, 0);
	//glClearDepth(0.0f); // 0.0f instead of 1.0f because reversed-z is used

	//

	glDrawArrays( MapDrawingPrimitive( m_drawingPrimitive ) , 0, m_nbverticesToRender );
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 1);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // default FBO
	glBlitFramebuffer(
		0, 0, WIDTH, HEIGHT,
		0, 0, WIDTH, HEIGHT,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	NEP_GRAPHICS_ASSERT();
}

void VAORenderer::bindShaderAttributes(const GraphicsProgram& pgm)
{
	glBindVertexArray( m_vao );
	Renderer::bindShaderAttributes( pgm );
}
