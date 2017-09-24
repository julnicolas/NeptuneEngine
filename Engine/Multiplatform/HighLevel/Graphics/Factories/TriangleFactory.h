#pragma once

#include "Graphics/Factories/PolygonFactory.h"

namespace Neptune
{
	class TriangleFactory final: public PolygonFactory
	{
	public:
		TriangleFactory(const Color& _color): PolygonFactory(_color)			{}
		TriangleFactory(const char* _texturePath): PolygonFactory(_texturePath)	{}

		virtual ~TriangleFactory()									= default;
		TriangleFactory(const TriangleFactory&)						= delete;
		virtual TriangleFactory& operator=(const TriangleFactory&)	= delete;

	protected:
		View* createViewAndSetUpRenderParameters()	final override;
		void createVertexData()						final override;
		void createTextureCoordinates()				final override;
	};
}