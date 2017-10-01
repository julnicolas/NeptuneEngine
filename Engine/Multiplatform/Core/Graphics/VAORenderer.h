#pragma once

#include "Graphics/Renderer.h"
#include <unordered_map>

namespace Neptune
{
	class VAORenderer : public Neptune::Renderer
	{
	public:
		VAORenderer();
		virtual ~VAORenderer()						= default;

		bool init()									  override; /// Allocate all the VBOs to store the vertex attributes
		bool cloneInit(const Renderer& _source)		  override;
		void terminate()							  override;

	protected:
		// Enable copy
		VAORenderer(const VAORenderer&)				= default;
		VAORenderer& operator=(const VAORenderer&)	= default;

		// Disable move
		VAORenderer(VAORenderer&&)					= delete;
		VAORenderer&& operator=(VAORenderer&&)		= delete;

		// Other methods
		void draw() override;                                                   /// Display graphics to the window
		void bindShaderAttributes(const GraphicsProgram& pgm) override;

	private:
		u32  m_vao;
	};
}