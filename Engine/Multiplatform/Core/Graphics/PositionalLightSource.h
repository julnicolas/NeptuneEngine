#pragma once

#include "Graphics/LightSource.h"
#include "Math/Vectors/Vec3.h"

namespace Neptune
{
	/// Base class for positional light sources such as point lights.

	// To do: fix vector interface so that a const ref can be passed

	class PositionalLightSource : public LightSource
	{
	public:
		PositionalLightSource(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f);
			
		virtual ~PositionalLightSource()                                            = 0;
		PositionalLightSource(const PositionalLightSource&)                         = default;
		virtual PositionalLightSource& operator=(const PositionalLightSource&)      = default;

		/// Get or set the max distance upon which the light has an effect
		float getMaxDistance() const           { return m_maxDistance;                   }
		void setMaxDistance(float d)           { m_maxDistance = d;                      }

		/// Get or set the light's position in World coordinates
		Vec3 getPos() const;
		void setPos(Vec3& pos);
		void setPos(float _x,float _y,float _z) { m_posX = _x; m_posY = _y; m_posZ = _z; }

	protected:
		float m_maxDistance;		/// Max distance at which the light cast rays

		/// Light's center position in World coordinates
		float  m_posX;
		float  m_posY;
		float  m_posZ;
	};
}