#pragma once

#include "Graphics/Factories/PolygonFactory.h"

namespace Neptune
{
	class CubeFactory final: public PolygonFactory
	{
	public:
		CubeFactory(const Color& _color): PolygonFactory(_color)			{}
		CubeFactory(const char* _texturePath): PolygonFactory(_texturePath)	{}

		virtual ~CubeFactory()								= default;
		CubeFactory(const CubeFactory&)						= delete;
		virtual CubeFactory& operator=(const CubeFactory&)	= delete;

	protected:
		View* createViewAndSetUpRenderParameters()	final override;
		void createVertexData()						final override;
		void createTextureCoordinates()				final override;
	};
}