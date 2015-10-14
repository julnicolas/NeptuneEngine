#pragma once

#include "StdInterface/Updatable.h"
#include "System/Type/Integers.h"
#include "Graphics/GraphicalProgram.h"

#include <vector>

namespace Neptune
{
	class Renderer : public Updatable
	{
	public:
		enum DrawingPrimitive : u8
		{
			TRIANGLES
		};

		// New interface

		Renderer();
		virtual ~Renderer();
		Renderer(const Renderer&)            = delete;
		Renderer& operator=(const Renderer&) = delete;

		GraphicalProgram& createProgram();
		void setNbverticesToRender(u32 nbvertices)     { m_nbverticesToRender = nbvertices; }
		void setDrawingPrimitve(DrawingPrimitive dp)   { m_drawingPrimitive   = dp;         }

		bool init()            override;
		bool update()    final override;
		void terminate()       override;

		// Old interface

		void setRenderingPgm(u32 pgm_id)           { m_renderPgm = pgm_id;              }
		void deleteRenderingProgram();					  
		void setVAO(u32 vao_handle);					  
		void deleteVAO();								  
		void setVertexIndicesType(u16 type)        { m_vertexIndicesType = type;        }

		void setBackgroundColor(float r, float g, float b, float a); // A value between 0.0f and 1.0f is expected for each parameter

		void setMVMatrix(const float mv[4][4]);
		void setProjMatrix(const float proj[4][4]);

	protected:

		// Types
		typedef std::vector<GraphicalProgram>::const_iterator ConstGraphicalProgramIterator;
		typedef std::vector<GraphicalProgram>::iterator       GraphicalProgramIterator;
		
		typedef std::vector<u32>::const_iterator              ConstGLResourceIterator;
		typedef std::vector<u32>::iterator                    GLResourceIterator;

		// Methods
		virtual void draw() = 0;                                             /// Display graphics to the window
		virtual void bindShaderAttributes(const GraphicalProgram& pgm) = 0;

		// Last version
		u32                           m_nbverticesToRender;  /// Number of vertices that will be passed to the vertex shader
		DrawingPrimitive              m_drawingPrimitive;    /// Which OpenGL drawing primitive must be used for rendering (Triangles, TriangleFan...)
		std::vector<u32>              m_glResourcesId;       /// Array of all the OpenGl resources' id allocated by the renderer. Its inner organization and what it contains (e.g. VAOs, VBOs...) are up to the specialized renderer.              
		std::vector<GraphicalProgram> m_programs;            /// Contains all the OpenGL programs that must be applied by a renderer


		// Previous version
		u32    m_renderPgm;
		u32    m_vertexArrayObject;
		u16    m_vertexIndicesType;
		float  m_backgroundColor[4];
		float  m_mvMatrix[4][4];
		float  m_projMatrix[4][4];

	private:
		void bindUniformVars(ConstGraphicalProgramIterator& it);
	};
}
