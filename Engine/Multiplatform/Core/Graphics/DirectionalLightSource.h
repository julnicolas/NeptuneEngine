#pragma once

#include "Graphics/PositionalLightSource.h"
#include "Math/Vectors/Vec3.h"

namespace Neptune
{
	/// Base class for directional light sources such as spot lights.
	
	// To do: fix vector interface so that a const ref can be passed

	class DirectionalLightSource : public PositionalLightSource
	{
	public:
		DirectionalLightSource(Vec3& _direction, float _x = 0.0f, float _y = 0.0f, float _z = 0.0f);

		virtual ~DirectionalLightSource()                                        = 0;
		DirectionalLightSource(const DirectionalLightSource&)                    = default;
		virtual DirectionalLightSource& operator=(const DirectionalLightSource&) = default;

		// Interface
		Vec3 getDirection() const;
		void setDirection(float _x, float _y, float _z) { m_dirX = _x; m_dirY = _y; m_dirZ = _z; }
		void setDirection(Vec3& _dir);

	protected:
		/// Vector along which the light source casts rays
		float m_dirX;
		float m_dirY;
		float m_dirZ;
	};
}