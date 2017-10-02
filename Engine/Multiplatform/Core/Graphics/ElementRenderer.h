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
		virtual ~ElementRenderer()								= default;

		bool init()												override; /// Allocate all the VBOs to store the vertex attributes
		bool cloneInit(const Renderer& _source)					override;
		void terminate()										override;

		void setIndexBufferData(const void* data, u32 dataSize, IndexType dataType);

	protected:
		// Enable copy
		ElementRenderer(const ElementRenderer&)					= default;
		ElementRenderer& operator=(const ElementRenderer&)		= default;

		// Disable move
		ElementRenderer(ElementRenderer&&)						= delete;
		ElementRenderer&& operator=(ElementRenderer&&)			= delete;

		// Other methods
		void draw() override;                                                   /// Display graphics to the window
		void bindShaderAttributes(const GraphicsProgram& pgm)	 override;

	private:
		u32				m_indexBufferID;
		IndexType		m_indexType;
		u32				m_indexBufferSize;
		const void*		m_indexBuffer;		/// \note The type is opaque as it can vary depending on the view
	};
}
