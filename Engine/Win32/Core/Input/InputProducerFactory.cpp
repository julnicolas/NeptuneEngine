#include "Input/InputProducerFactory.h"
#include "Input/SDLInputProducer.h"
#include "Debug/NeptuneDebug.h"


using namespace Neptune;
using DefaultProducerType = SDLInputProducer;


InputProducer* InputProducerFactory::CreateDefaultProducer()
{
	DefaultProducerType* default_producer = new DefaultProducerType;
	
	return default_producer;
}

void InputProducerFactory::DestructDefaultProducer(InputProducer* _producer)
{
	NEP_ASSERT_ERR_MSG(_producer != nullptr, "Wrong pointer. Pointer is nullptr.");
	NEP_ASSERT_ERR_MSG(dynamic_cast<DefaultProducerType*>(_producer) != nullptr, "Type mismatch - _producer is not of type DefaultProducerType.\
																				 Have a look at InputProducerFactory.cpp for more information.");
	
	delete _producer;
}