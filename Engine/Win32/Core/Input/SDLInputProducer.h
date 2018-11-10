#pragma once

#include "Input/InputProducer.h"


namespace Neptune
{
	class SDLInputProducer final : public InputProducer
	{
	public:
		SDLInputProducer()										=default;
		virtual ~SDLInputProducer()								=default;
		SDLInputProducer(const SDLInputProducer&)				=delete;
		SDLInputProducer& operator=(const SDLInputProducer&)	=delete;

	protected:
		void fetchInputs() final override;
	};
}