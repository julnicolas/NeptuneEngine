#pragma once

#include "Input/InputTypes.h"
#include <unordered_map>
#include <vector>


namespace Neptune
{
	class InputConsumer;

	/// \warning If a consumer refers to a destructed producer, the system will crash.
	/// Therefore make sure InputProducer last longer than their consumers.
	class InputProducer
	{
	public:
		InputProducer()									= default;
		virtual ~InputProducer()						= default;
		InputProducer(const InputProducer&)				= delete;
		InputProducer& operator=(const InputProducer&)	= delete;

		/// Adds a consumer to the consumer list. It subscribes to _type.
		void addConsumer(InputConsumer* _consumer, InputType _type);
		/// Removes a consumer from the consumer list for the input _type.
		void rmConsumer(InputConsumer* _consumer, InputType _type);
		/// Fetches inputs and publish them to its subscribers.
		void update();

	protected:
		/// Fetches all available inputs from the source
		virtual void fetchInputs() =0;

		/// Returns true if there are any subscribers for the input type _type
		bool anySubscribersFor(InputType _type) { return m_input_list.find(_type) != m_input_list.end(); }

		/// Pushes a single event to the input queue.
		void pushInput(InputType _type, Input&& _input);

		/// Appends inputs to the input queue. Inputs are guaranteed to be copied.
		/// The copy constructor is called for objects and scalars are moved to the input queue.
		/// \param _type Input's type
		/// \param _inputArray Array of inputs to be pushed to the queue
		/// \param _size Input array's size
		void pushInputs(InputType _type, Input* _inputArray, u32 _size);

	private:
		/// Publish inputs to the consumers. Inputs from m_input_list are cleared after this methods finishes.
		void publish();

		/// Contains all fetched inputs. The content is flushed on every update.
		std::unordered_map<InputType, std::vector<Input>> m_input_list;

		/// Contains all input consumers. Ordered by input type to ease publication.
		std::unordered_map<InputType, std::vector<InputConsumer*>> m_consumer_list;
	};
}