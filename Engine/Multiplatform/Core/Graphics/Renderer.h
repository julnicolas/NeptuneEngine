#pragma once

#include "StdInterface/Updatable.h"
#include "System/Type/Integers.h"
#include "Graphics/GraphicsProgram.h"

#include <vector>

namespace Neptune
{
	class Renderer : public Updatable
	{
	public:
		typedef size_t ProgramID;

		enum DrawingPrimitive : u8
		{
			TRIANGLES,
			TRIANGLE_STRIP,
			TRIANGLE_FAN
		};

		// New interface

		Renderer();
		virtual ~Renderer();
		Renderer(const Renderer&)            = delete;
		Renderer& operator=(const Renderer&) = delete;

		///
		/// \brief   Adds a graphics program to the renderer's program table. Once added (given that the program has been set up properly), it is ready for use. 
		/// \return  The program's ID within the renderer's program table.
		/// \warning It is YOUR responsibility for the program to remain in memory for the whole operation of the renderer
		///
		ProgramID	addProgram           (GraphicsProgram* _pgm);
		bool		updateUniform        (u8 pgm_index, const char* name, const void* data);
		void		setNbverticesToRender(u32 nbvertices)										{ m_nbverticesToRender = nbvertices; }
		void		setDrawingPrimitive  (DrawingPrimitive dp)									{ m_drawingPrimitive   = dp;         }

		bool		init()            override;
		bool		update()    final override;
		void		terminate()       override;

	protected:
		// Types
		typedef std::vector<GraphicsProgram*>::const_iterator ConstGraphicalProgramIterator;
		typedef std::vector<GraphicsProgram*>::iterator       GraphicalProgramIterator;

		// Methods
		virtual void draw() = 0;																									/// Display graphics to the window
		virtual void bindShaderAttributes(const GraphicsProgram& pgm);

		u32                                                           m_nbverticesToRender;											/// Number of vertices that will be passed to the vertex shader
		DrawingPrimitive                                              m_drawingPrimitive;											/// Which OpenGL drawing primitive must be used for rendering (Triangles, TriangleFan...)
		std::vector<GraphicsProgram*>                                 m_programs;													/// Contains all the OpenGL programs that must be applied by a renderer
		std::unordered_map<const GraphicsProgram*,std::vector<u32> >  m_vbos;														/// Contains all vbos used by a graphics program

	private:
		void bindUniformVars(ConstGraphicalProgramIterator& it);
		void bindTextures(const GraphicsProgram& _pgm);
	};
}
