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
		std::unordered_map<InputType, std::vector<Input>>::iterator it = m_input_list.find(InputType::ANY);
		if (it != m_input_list.end())
			it->second.push_back(std::move(static_cast<Input>(sdl_event)));

		// Check if anyone subscribes to a more specific input type
		if (input_type != InputType::ANY)
		{
			it = m_input_list.find(input_type);
			if (it != m_input_list.end())
				it->second.push_back(std::move(static_cast<Input>(sdl_event)));
		}
	}
}