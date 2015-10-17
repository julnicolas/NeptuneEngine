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

		void setBackgroundColor(float r, float g, float b, float a); // A value between 0.0f and 1.0f is expected for each parameter

	protected:
		// Types
		typedef std::vector<GraphicalProgram>::const_iterator ConstGraphicalProgramIterator;
		typedef std::vector<GraphicalProgram>::iterator       GraphicalProgramIterator;

		// Methods
		virtual void draw() = 0;                                             /// Display graphics to the window
		virtual void bindShaderAttributes(const GraphicalProgram& pgm) = 0;

		// Last version
		u32                           m_nbverticesToRender;  /// Number of vertices that will be passed to the vertex shader
		DrawingPrimitive              m_drawingPrimitive;    /// Which OpenGL drawing primitive must be used for rendering (Triangles, TriangleFan...)
		std::vector<GraphicalProgram> m_programs;            /// Contains all the OpenGL programs that must be applied by a renderer

		// Previous version

		float  m_backgroundColor[4];

	private:
		void bindUniformVars(ConstGraphicalProgramIterator& it);
	};
}
