#include "Renderer.h"
#include "Debug/NeptuneDebug.h"
#include <cstring>

#ifdef NEP_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	
	#include <GL/glew.h>
#endif

// U T I L I T Y   F U N C T I O N S


static void setAsIdentityMatrix(GLfloat m[4][4])
{
	m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
	m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
	m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}

// R E N D E R E R ' S   M E T H O D S


Neptune::Renderer::Renderer():
	m_renderPgm(0), m_vertexArrayObject(0),
	m_vertexIndicesType(0)
{
	setBackgroundColor( 0.0f, 0.0f, 0.0f, 1.0f );
	setAsIdentityMatrix( m_mvMatrix   );
	setAsIdentityMatrix( m_projMatrix );
}

Neptune::Renderer::~Renderer()
{
	deleteRenderingProgram();
	//deleteVAO();
}

void Neptune::Renderer::setVAO(u32 vao_handle)
{
	m_vertexArrayObject = vao_handle;
}

void Neptune::Renderer::deleteVAO()
{
	if (m_vertexArrayObject)
		glDeleteVertexArrays(1, &m_vertexArrayObject);
	else
		NEP_ASSERT(false);
}

void Neptune::Renderer::setBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	m_backgroundColor[0] = r; m_backgroundColor[1] = g;
	m_backgroundColor[2] = b; m_backgroundColor[3] = a;
}

void Neptune::Renderer::setMVMatrix(const GLfloat mv[4][4])
{
	memcpy( m_mvMatrix, mv, 16*sizeof(GLfloat) );
}

void Neptune::Renderer::setProjMatrix(const GLfloat proj[4][4])
{
	memcpy( m_projMatrix, proj, 16*sizeof(GLfloat) );
}

void Neptune::Renderer::sendCTMMatrix()
{
	// MV
	GLint location = glGetUniformLocation(m_renderPgm, "MV");
	NEP_ASSERT( location != GL_INVALID_VALUE && location != GL_INVALID_OPERATION );

	if ( location >= 0 )
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_mvMatrix[0][0]);

	// Proj
	location = glGetUniformLocation(m_renderPgm, "Proj");

	if ( location >= 0 )
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_projMatrix[0][0]);
}

void Neptune::Renderer::draw()
{
	// Screen cleaning
	glClearBufferfv(GL_COLOR, 0, m_backgroundColor);
	glClear( GL_DEPTH_BUFFER_BIT );

	// Prepare vertex shader
	glBindVertexArray(m_vertexArrayObject);
	glUseProgram(m_renderPgm);
	sendCTMMatrix();

	// Draw-call
	glDrawElements(GL_TRIANGLES, m_nbverticesToRender, m_vertexIndicesType, 0); // Watch out! the third value corresponds to the vertex-index type
	//glDrawArrays(/*GL_PATCHES*/ GL_TRIANGLES, 0, 3*12); // GL_TRIANGLES without any tess shader
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Only draws the edges
}

void Neptune::Renderer::deleteRenderingProgram()
{
	if (m_renderPgm)
		glDeleteProgram(m_renderPgm);
	else
		NEP_ASSERT(false);
}