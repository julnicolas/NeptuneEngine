#ifndef MOST_BASIC_RENDERER_H_
#define MOST_BASIC_RENDERER_H_

#include "System/Type/Integers.h"

namespace Neptune
{
	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		// Display graphics to the window
		void draw();

		void setRenderingPgm(u32 pgm_id)           { m_renderPgm = pgm_id;              }
		void deleteRenderingProgram();					  
		void setVAO(u32 vao_handle);					  
		void deleteVAO();								  
		void setNbverticesToRender(u32 nbvertices) { m_nbverticesToRender = nbvertices; }
		void setVertexIndicesType(u16 type)        { m_vertexIndicesType = type;        }

		void setBackgroundColor(float r, float g, float b, float a); // A value between 0.0f and 1.0f is expected for each parameter

		void setMVMatrix(const float mv[4][4]);
		void setProjMatrix(const float proj[4][4]);

	protected:
		u32    m_renderPgm;
		u32    m_vertexArrayObject;
		u32    m_nbverticesToRender;
		u16    m_vertexIndicesType;
		float  m_backgroundColor[4];
		float  m_mvMatrix[4][4];
		float  m_projMatrix[4][4];

	private:
		void sendCTMMatrix(); // CTM stands for Current Transform Matrix = Proj*MV

		// Forbidden use
		Renderer(const Renderer&){}
		Renderer& operator=(const Renderer&) {return *this;}
	};
}

#endif