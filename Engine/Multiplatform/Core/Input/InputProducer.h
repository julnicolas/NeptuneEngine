#pragma once

#include <unordered_map>
#include <vector>
#include "Input/InputTypes.h"


namespace Neptune
{
	class InputConsumer;

	class InputProducer
	{
	public:
		InputProducer()									= default;
		/// Unsubscribe every consumers
		virtual ~InputProducer();
		InputProducer(const InputProducer&)				= delete;
		InputProducer& operator=(const InputProducer&)	= delete;

		/// Adds a consumer to the consumer list. It subscribes to _type.
		void addConsumer(InputConsumer* _consumer, InputType _type);
		/// Removes a consumer from the consumer list for the input _type.
		/// \return Returns false if the provided consumer doesn't subscribe 
		/// to the input '_type'. True otherwise. 
		bool rmConsumer(InputConsumer* _consumer, InputType _type);
		/// Fetches inputs and publish them to its subscribers.
		bool update();

	protected:
		/// Fetches all available inputs from the source
		virtual void fetchInputs() =0;

		/// Contains all fetched inputs. The content is flushed on every update.
		std::unordered_map<InputType, std::vector<Input>> m_input_list;
	private:
		/// Publish inputs to the consumers
		void publish();

		/// Contains all input consumers. Ordered by input type to ease publication.
		std::unordered_map<InputType, std::vector<InputConsumer*>> m_consumer_list;
	};
}