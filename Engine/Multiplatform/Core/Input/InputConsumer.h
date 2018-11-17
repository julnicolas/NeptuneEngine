#pragma once

#include "Input/InputTypes.h"
#include <unordered_map>
#include <vector>
#include <functional>

namespace Neptune
{
	class InputProducer;

	class InputConsumer
	{
	public:
		InputConsumer()									:m_producer(nullptr){}
		~InputConsumer()								= default;
		InputConsumer(const InputConsumer&)				= delete;
		InputConsumer(InputConsumer&&);					// TODO: set to =default; not the case because not compatible with vs2013
		InputConsumer& operator=(const InputConsumer&)	= delete;
		InputConsumer& operator=(InputConsumer&&);		// TODO: set to =default; not the case because not compatible with vs2013

		/// Callback function summoned when update is called. A callback takes action on a specified input type.
		/// \note Callback functions should have the following signature - void myFunc(const Input&)
		/// Please, also note that it is possible to pass lambda-closures.
		using InputCallback = std::function<void(const Input&)>;

		/// Adds the current InputConsumer to the producer's consumer list.
		/// A consumer callback must be added so that inputs don't pile up indefinitely.
		void subscribe(InputProducer* _producer, InputType _type, InputCallback _callback);
		/// Removes the current InputConsumer from the producer's consumer list.
		/// All callbacks bound to _type are automatically removed.
		void cancelSubscription(InputType _type);
		/// Gets the producer the object is subscribed to
		const InputProducer* getProducer() const { return m_producer; }
		/// Pushes a list of events to the queue of _type events
		void push(InputType _type, Input* _inputArray, u32 _length);
		
		/// Executes the callbacks on the inputs received in the input queue.
		/// Callbacks are executed in the order they were added.
		void update();

		/// Adds a callback to the callback list. Callbacks are mapped to input types.
		/// \param _callback Callback function to be called for the provided InputType
		/// \param _type The provided callback will be called for the InputType _type
		/// \return A resource ID corresponding to the callback
		/// \note The ID generated will be the same if users add a callback, remove it 
		///  and then add the same callback again.
		u32 add(InputCallback _callback, InputType _type);

		/// Removes a callback from the callback list.
		/// \param _callbackID The callback id returned by add
		/// \param _type The input type for which the callback must removed
		/// \note make sure to cancel subscriptions when removing the last callbacks of each type, otherwise inputs will stack up.
		void remove(u32 _callbackID, InputType _type);

	private:
		/// Callback storage struct
		struct InputCallbackInfo
		{
			InputCallback	m_callback;		/// Callback function, functor or closure
			u32				m_callbackID;	/// Resource ID, useful for deletion by users
		};

		InputProducer* m_producer;
		std::unordered_map<InputType, std::vector<InputCallbackInfo>> m_callback_map;
		std::unordered_map<InputType, std::vector<Input>> m_input_queue;
	};
}