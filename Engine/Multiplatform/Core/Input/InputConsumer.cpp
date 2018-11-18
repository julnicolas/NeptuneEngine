#include "Input/InputConsumer.h"
#include "Input/InputProducer.h"
#include "System/Hashing/FastHashFunctions.h"
#include "Debug/NeptuneDebug.h"
#include <type_traits>
#include <algorithm>

using namespace Neptune;


// Must be defaulted
InputConsumer::InputConsumer(InputConsumer&& _obj)
{
	this->m_producer		= _obj.m_producer;
	this->m_callback_map	= std::move(_obj.m_callback_map);
	this->m_input_queue		= std::move(_obj.m_input_queue);
}

// Must be defaulted
InputConsumer& InputConsumer::operator=(InputConsumer&& _obj)
{
	this->m_producer		= _obj.m_producer;
	this->m_callback_map	= std::move(_obj.m_callback_map);
	this->m_input_queue		= std::move(_obj.m_input_queue);

	return *this;
}

void InputConsumer::subscribe(InputProducer* _producer, InputType _type, InputCallback _callback)
{
	// Subscribe to the producer
	NEP_ASSERT_ERR_MSG(_producer != nullptr, "Invalid pointer for _producer.");
	m_producer = _producer;
	m_producer->addConsumer(this, _type);
	
	// Create the queue to receive its inputs (if it doesn't already exist)
	m_input_queue.emplace(_type, std::vector<Input>());

	// Add the callback to react to inputs
	add(_callback, _type);
}

void InputConsumer::cancelSubscription(InputType _type)
{
	// Cancel the subscription to the producer
	NEP_ASSERT_ERR_MSG(m_producer != nullptr, "The object hasn't subscribed to any consumer.");
	m_producer->rmConsumer(this, _type);
	
	// Deallocate the memory used to store _type inputs (as none will arrive)
	m_input_queue.erase(_type);

	// Remove all callbacks associated to _type
	m_callback_map.erase(_type);

	// If there is not any subscriptions left, then definitely remove the producer
	if (m_input_queue.empty())
		m_producer = nullptr;
}

void InputConsumer::push(InputType _type, Input* _inputArray, u32 _length)
{
	NEP_ASSERT_ERR_MSG(m_input_queue.find(_type) != m_input_queue.end(), "Input type cannot be found.");
	std::vector<Input>& input_list = m_input_queue[_type];
	input_list.insert(input_list.end(), _inputArray, _inputArray + _length); // This is not a mistake - the end iterator points past the last index
}

void InputConsumer::update()
{
	// For each input queue, execute all the corresponding callbacks on all its inputs (FIFO order)
	for (auto& type_input_queue_pair : m_input_queue)
	{
		// For each input call every callbacks
		for (Input& input : type_input_queue_pair.second)
		{
			// For each callback mapped to the current queue (same type), call the callback on the current input. 
			for (const auto& callback_info : m_callback_map[type_input_queue_pair.first])
			{
				callback_info.m_callback(input);
			}
		}

		// Once every callbacks have been called on every inputs, clear the queue
		type_input_queue_pair.second.clear();
	}
}

u32 InputConsumer::add(InputCallback _callback, InputType _type)
{
	NEP_ASSERT_ERR_MSG(_callback != nullptr, "Invalid callback, value is nullptr");
	
	// Prepare the callback for insertion
	InputCallbackInfo callback_info;
	callback_info.m_callback			= _callback;
	callback_info.m_callbackID			= 0; // Not computed yet

	// Create callback list for _type if not existent, then add a callback in any cases.
	auto& callback_list = m_callback_map[_type];
	auto it = callback_list.insert(callback_list.end(), callback_info);

	// Generate an ID
	// The ID generated will be the same if the user adds a callback, removes it and then adds the same callback again.
	static_assert(std::is_same<decltype(callback_list), std::vector<InputCallbackInfo>&>::value, "Invalid type for generating resource ID");
	NEP_LOG("Generating resource ID");
	u32 id = Fnv1a32(reinterpret_cast<u8*>(callback_list.data()), callback_list.size()*sizeof(callback_list[0]));
	NEP_LOG("Done generating resource ID : %u", id);

	// Set the inserted element's ID
	it->m_callbackID = id;

	return id;
}

void InputConsumer::remove(u32 _callbackID, InputType _type)
{
	NEP_ASSERT_ERR_MSG(m_callback_map.find(_type) != m_callback_map.end(), "No callback registered");
	
	// Remove the callback from the callback list
	std::vector<InputCallbackInfo>& callback_info_list = m_callback_map[_type];
	std::vector<InputCallbackInfo>::iterator it = std::find_if(	callback_info_list.begin(),
																callback_info_list.end(),
																[_callbackID](const InputCallbackInfo& _info){
																	return _info.m_callbackID == _callbackID;
																});
	
	NEP_ASSERT_ERR_MSG(it != callback_info_list.end(), "Callback not found");
	callback_info_list.erase(it);

	// Remove the list if no callback is present
	if (callback_info_list.empty())
		m_callback_map.erase(_type);
}