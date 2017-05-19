#pragma once

#include "Graphics/LightSource.h"

namespace Neptune
{
	class AmbientLight : public LightSource
	{
	public:
		AmbientLight(const Color& _color){ m_color = _color; }
		virtual ~AmbientLight()                      = default;

		AmbientLight(const AmbientLight&)            = default;
		AmbientLight& operator=(const AmbientLight&) = default;
	};
}