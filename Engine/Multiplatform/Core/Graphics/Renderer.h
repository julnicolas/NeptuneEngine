#ifndef MOST_BASIC_RENDERER_H_
#define MOST_BASIC_RENDERER_H_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <GL/glew.h>

namespace Neptune
{
	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		// Display graphics to the window
		void draw();

		void setRenderingPgm(GLuint pgm_id)               { m_renderPgm = pgm_id;              }
		void deleteRenderingProgram();					  
		void setVAO(GLuint vao_handle);					  
		void deleteVAO();								  
		void setNbverticesToRender(GLuint64 nbvertices)   { m_nbverticesToRender = nbvertices; }
		void setVertexIndicesType(GLushort type)          { m_vertexIndicesType = type;        }

		void setBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a); // A value between 0.0f and 1.0f is expected for each parameter

		void setMVMatrix(const GLfloat mv[4][4])     { memcpy( m_mvMatrix, mv, 16*sizeof(GLfloat) );     }
		void setProjMatrix(const GLfloat proj[4][4]) { memcpy( m_projMatrix, proj, 16*sizeof(GLfloat) ); }

	protected:
		GLuint   m_renderPgm;
		GLuint   m_vertexArrayObject;
		GLuint64 m_nbverticesToRender;
		GLushort m_vertexIndicesType;
		GLfloat  m_backgroundColor[4];
		GLfloat  m_mvMatrix[4][4];
		GLfloat  m_projMatrix[4][4];

	private:
		void sendCTMMatrix(); // CTM stands for Current Transform Matrix = Proj*MV

		// Forbidden use
		Renderer(const Renderer&){}
		Renderer& operator=(const Renderer&){return *this;}
	};
}

#endif