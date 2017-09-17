#pragma once

#include "Graphics/Factories/PolygonFactory.h"

namespace Neptune
{
	class PlanFactory final: public PolygonFactory
	{
	public:
		PlanFactory(const Color& _color): PolygonFactory(_color)			{}
		PlanFactory(const char* _texturePath): PolygonFactory(_texturePath)	{}

		virtual ~PlanFactory()								= default;
		PlanFactory(const PlanFactory&)						= delete;
		virtual PlanFactory& operator=(const PlanFactory&)	= delete;

	protected:
		View* createViewAndSetUpRenderParameters()	final override;
		void createVertexData()						final override;
		void createTextureCoordinates()				final override;
	};
}