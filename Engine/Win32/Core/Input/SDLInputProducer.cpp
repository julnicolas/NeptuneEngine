#include "Input/SDLInputProducer.h"
#include "SDL2/SDL.h"


using namespace Neptune;

static InputType MapSDLEventToNeptuneInputType(const SDL_Event& _event)
{
	switch (_event.type)
	{
	case SDL_KEYDOWN:
		return InputType::KEYBOARD_PUSH;

	case SDL_KEYUP:
		return InputType::KEYBOARD_RELEASE;

	case SDL_WINDOWEVENT:
		return InputType::WINDOW;

	default:
		return InputType::ANY;
	}
}

void SDLInputProducer::fetchInputs()
{
	SDL_Event sdl_event;

	// If an event has been received, then redirect it to the
	// right event queues. The event is multi casted.
	if (SDL_PollEvent(&sdl_event) > 0)
	{
		InputType input_type = MapSDLEventToNeptuneInputType(sdl_event);

		// Check if any consumers subscribe to InputType::ANY (get every input)
		if (anySubscribersFor(InputType::ANY))
			pushInput(InputType::ANY, static_cast<Input>(sdl_event));

		// Check if anyone subscribes to a more specific input type
		if (input_type != InputType::ANY)
		{
			if (anySubscribersFor(input_type))
				pushInput(input_type, static_cast<Input>(sdl_event));
		}
	}
}