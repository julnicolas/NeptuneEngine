#pragma once

#include "Math/Vectors/Vec3.h"
#include "Math/Vectors/Vec4.h"
#include "Graphics/Color.h"

namespace Neptune
{
	class LightSource
	{
	public:
		LightSource(float x = 0.0f, float y = 0.0f, float z = 0.0f);
		virtual ~LightSource() {}

		LightSource(const LightSource&)	= default;
		virtual LightSource& operator=(const LightSource& obj);

		// Class' interface

		/// Get the ray casted for an object at position pos.
		/// The last component is the ray's intensity (between [0,1])
		virtual Vec4 getRay(const Vec3& pos) const =0;

		float getMaxDistance() const             { return m_maxDistance;														}
		void setMaxDistance(float d)             { m_maxDistance = d;															}
		
		const Color& getColor() const             { return m_color;																}
		void setColor(const Color& color)         { m_color = color;															}
		void setColor(float r, float g, float b, float a = 1.0f)  { m_color.r = r; m_color.g = g; m_color.b = b; m_color.a = a; }

		float getBaseIntensity() const { return m_baseIntensity; }
		void setBaseIntensity(float i) { m_baseIntensity = i; }

		/// Get or set the light's position in World coordinates
		const Vec3& getPos() const             { return m_position;												}
		void setPos(const Vec3& pos)           { m_position = pos;												}
		void setPos(float x, float y, float z) { m_position.x() = x; m_position.y() = y; m_position.z() = z;	}

	protected:
		float m_maxDistance;		/// Max distance at which the light cast rays (convention: any negative value for infinity)
		float m_baseIntensity;		/// Value between [0,1] denoting the light's 'strength' (strong or weak)
		Vec3  m_position;			/// Light's center position in World coordinates
		Color m_color;				/// Light's color
	};
}