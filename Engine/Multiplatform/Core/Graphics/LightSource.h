#pragma once

#include "System/Type/Integers.h"
#include "Graphics/Color.h"

namespace Neptune
{
	class LightSource
	{
	public:
		LightSource();
		virtual ~LightSource()                                 = default;

		LightSource(const LightSource&)	                       = default;
		virtual LightSource& operator=(const LightSource& obj) = default;

		// Class' interface
		
		const Color& getColor() const                             { return m_color;												}
		void setColor(const Color& color)                         { m_color = color;											}
		void setColor(float r, float g, float b, float a = 1.0f)  { m_color.r = r; m_color.g = g; m_color.b = b; m_color.a = a; }

		float getBaseIntensity() const                            { return m_baseIntensity;                                     }
		void setBaseIntensity(float i)                            { m_baseIntensity = i;                                        }

	protected:
		float m_baseIntensity;		/// Value between [0,1] denoting the light's 'strength' (strong or weak)
		Color m_color;				/// Light's color
	};
}