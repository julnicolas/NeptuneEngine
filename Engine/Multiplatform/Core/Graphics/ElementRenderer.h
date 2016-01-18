#pragma once

#include "Graphics/Renderer.h"
#include <unordered_map>

namespace Neptune
{
	class ElementRenderer: public Neptune::Renderer
	{
	public:
		enum IndexType : u8
		{
			U8,
			U16,
			U32
		};

		ElementRenderer();
		virtual ~ElementRenderer();
		ElementRenderer(const ElementRenderer&)            = delete;
		ElementRenderer& operator=(const ElementRenderer&) = delete;

		bool init()      override; /// Allocate all the VBOs to store the vertex attributes
		void terminate() override;

		void setIndexBufferData(const void* data, u32 dataSize, IndexType dataType);

	protected:
		void draw() override;                                                   /// Display graphics to the window
		void bindShaderAttributes(const GraphicalProgram& pgm) override;

	private:
		u32                                                           m_indexBuffer;
		IndexType                                                     m_indexType;
	};
}
