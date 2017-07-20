#pragma once

#include "System/Type/Integers.h"
#include "Graphics/Color.h"

namespace Neptune
{
	/// Light sources' base class.
	/// \brief This class could be seen as an ambient light source. However, this is just supposed to be the common
	/// \brief behavior of any lights in the engine. People should specialize the abstract light sources to implement
	/// \brief custom/well known behaviors.

	class LightSource
	{
	public:
		LightSource();
		virtual ~LightSource()                                 = 0;

		LightSource(const LightSource&)	                       = default;
		virtual LightSource& operator=(const LightSource& obj) = default;

		// Class' interface
		
		const Color& getColor() const                                     { return m_color;												}
		void         setColor(const Color& color)                         { m_color = color;											}
		void         setColor(float r, float g, float b, float a = 1.0f)  { m_color.r = r; m_color.g = g; m_color.b = b; m_color.a = a; }

		float        getBaseIntensity() const                             { return m_color.a;                                           }
		void         setBaseIntensity(float i)                            { m_color.a = i;                                              }

	protected:
		Color m_color;				/// Light's color. The last component is the light's intensity.
	};
}