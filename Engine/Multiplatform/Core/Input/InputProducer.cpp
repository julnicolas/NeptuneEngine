#include "Input/InputProducer.h"
#include "Debug/NeptuneDebug.h"
#include <algorithm>


using namespace Neptune;


void InputProducer::addConsumer(InputConsumer* _consumer, InputType _type)
{
	NEP_ASSERT_ERR_MSG(_consumer != nullptr, "wrong pointer, _consumer == nullptr");

	// Add the consumer to the consumer list. Consumers are sorted by event types.
	m_consumer_list[_type].push_back(_consumer);

	// Instantiate an input queue if it doesn't already exist
	m_input_list.emplace(_type);
}

void InputProducer::rmConsumer(InputConsumer* _consumer, InputType _type)
{
	NEP_ASSERT(_consumer != nullptr, "wrong pointer, _consumer == nullptr");
	
	// Delete the consumer so that it doesn't subscribe to the input feed any more
	std::vector<InputConsumer*>& consumers = m_consumer_list[_type];
	std::vector<InputConsumer*>::iterator it = std::find(consumers.begin(), consumers.end(), _consumer);
	NEP_ASSERT_ERR_MSG(it != consumers.end(), "Couldn't find consumer in m_consumer_list");

	consumers.erase(it);

	// If the consumer list is empty, it means they are not any subscribers for
	// the input of type _type. So the associated input queue must be deleted.
	if (consumers.empty())
		m_input_list.erase(_type);
}

void InputProducer::update()
{
	fetchInputs();
	publish();
}

void InputProducer::publish()
{
	NEP_ASSERT_ERR_MSG(false, "not implemented");
}
