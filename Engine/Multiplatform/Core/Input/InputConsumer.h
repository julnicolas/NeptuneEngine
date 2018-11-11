#pragma once

#include <unordered_map>
#include <vector>
#include "Input/InputTypes.h"

namespace Neptune
{
	class InputProducer;

	class InputConsumer
	{
	public:
		InputConsumer()									:m_producer(nullptr){}
		~InputConsumer()								= default;
		InputConsumer(const InputConsumer&)				= delete;
		InputConsumer(InputConsumer&&);
		InputConsumer& operator=(const InputConsumer&)	= delete;
		InputConsumer& operator=(InputConsumer&&);

		/// Callback function summoned when update is called. A callback takes action on a specified input type.
		/// \example void myFunc(const Input&)
		typedef void(*InputCallback)(const Input&);

		/// Adds the current InputConsumer to the producer's consumer list
		void subscribe(InputProducer* _producer, InputType _type);
		/// Removes the current InputConsumer from the producer's consumer list
		void cancelSubscription(InputProducer* _producer, InputType* _type, u32 nb_types);
		/// Gets the producer the object is subscribed to
		const InputProducer* getProducer() const { return m_producer; }
		
		/// Executes the callbacks on the inputs received in the input queue
		void update();

		/// Adds a callback to the callback list. Callbacks are mapped to input types. 
		void add(InputCallback _callback, InputType _type);
		/// Removes a callback from the callback list 
		void remove(InputCallback _callback);

	private:
		InputProducer* m_producer;
		std::unordered_map<InputType, std::vector<InputCallback>> m_callback_map;
		std::unordered_map<InputType, std::vector<Input>> m_input_queue;
	};
}