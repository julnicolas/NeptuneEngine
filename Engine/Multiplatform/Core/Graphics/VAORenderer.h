#pragma once

#include "Graphics/Renderer.h"
#include <unordered_map>

namespace Neptune
{
	class VAORenderer : public Neptune::Renderer
	{
	public:
		VAORenderer();
		virtual ~VAORenderer();
		VAORenderer(const VAORenderer&)            = delete;
		VAORenderer& operator=(const VAORenderer&) = delete;

		bool init()      override; /// Allocate all the VBOs to store the vertex attributes
		void terminate() override;

	protected:
		void draw() override;                                                   /// Display graphics to the window
		void bindShaderAttributes(const GraphicalProgram& pgm) override;

	private:
		u32                                                            m_vao;
		std::unordered_map<const GraphicalProgram*, std::vector<u32> > m_vbos; /// Contains all vbos used by a graphical program
	};
}