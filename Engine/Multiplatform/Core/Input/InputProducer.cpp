#include "Input/InputProducer.h"
#include "Debug/NeptuneDebug.h"
#include <algorithm>


using namespace Neptune;


void InputProducer::addConsumer(InputConsumer* _consumer, InputType _type)
{
	m_consumer_list[_type].push_back(_consumer);
}

void InputProducer::rmConsumer(InputConsumer* _consumer, InputType _type)
{
	std::vector<InputConsumer*>& consumers = m_consumer_list[_type];
	std::vector<InputConsumer*>::iterator it = std::find(consumers.begin(), consumers.end(), _consumer);
	NEP_ASSERT_ERR_MSG(it != consumers.end(), "Couldn't find consumer in m_consumer_list");

	consumers.erase(it);
}

void InputProducer::update()
{
	fetchInputs();
	publish();
}

void InputProducer::publish()
{

}
