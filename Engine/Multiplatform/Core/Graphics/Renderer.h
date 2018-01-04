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
			TRIANGLE_FAN,
			QUADS,
			QUAD_STRIP
		};

		Renderer();
		virtual ~Renderer()		= default;

		///
		/// \brief   Adds a graphics program to the renderer's program table. Once added (given that the program has been set up properly), it is ready for use. 
		/// \return  The program's ID within the renderer's program table.
		/// \warning It is YOUR responsibility for the program to remain in memory for the whole operation of the renderer
		///
		ProgramID	addProgram           (GraphicsProgram* _pgm);
		bool		updateUniform        (u8 pgm_index, const char* name, const void* data);
		void		setNbverticesToRender(u32 nbvertices)										{ m_nbverticesToRender = nbvertices; }
		void		setDrawingPrimitive  (DrawingPrimitive dp)									{ m_drawingPrimitive   = dp;         }

		bool			init()								override;

		/// \brief		Initialize the renderer so that it shares the same rendering parameters as _source (i.e. buffers and handles).
		/// \warning	It will become tightly coupled to _source as they'll share most of their rendering parameters.
		///				As a consequence, terminate must only be called once and only once for all clones and _source (any objects can call it
		///				but I would suggest to let _source make the call for the sake of clarity).
		virtual bool	cloneInit(const Renderer& _source);						
		bool			update()							final override;
		void			terminate()							override;

	protected:
		// T Y P E S
		typedef std::vector<GraphicsProgram*>::const_iterator ConstGraphicalProgramIterator;
		typedef std::vector<GraphicsProgram*>::iterator       GraphicalProgramIterator;

		// M E T H O D S
		// Allow copy
		Renderer(const Renderer&)							= default;
		Renderer& operator=(const Renderer&)				= default;

		// Disable move
		Renderer(Renderer&&)								= delete;
		Renderer&& operator=(Renderer&&)					= delete;

		virtual void draw() = 0;																									/// Display graphics to the window
		virtual void bindShaderAttributes(const GraphicsProgram& pgm);

		// A T T R I B U T E S
		u32																m_nbverticesToRender;											/// Number of vertices that will be passed to the vertex shader
		DrawingPrimitive												m_drawingPrimitive;											/// Which OpenGL drawing primitive must be used for rendering (Triangles, TriangleFan...)
		std::vector<GraphicsProgram*>									m_programs;													/// Contains all the OpenGL programs that must be applied by a renderer
		std::unordered_map<const GraphicsProgram*,std::vector<u32> >	m_vbos;														/// Contains all vbos used by a graphics program
		u32*															m_vboHandles;

	private:
		void bindUniformVars(ConstGraphicalProgramIterator& it);
		void bindTextures(GraphicsProgram& _pgm);
	};
}
