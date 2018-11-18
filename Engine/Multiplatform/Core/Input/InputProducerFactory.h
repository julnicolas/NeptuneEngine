#pragma once


namespace Neptune
{
	class InputProducer;

	namespace InputProducerFactory
	{
		/// Creates a platform-dependent default InputProducer.
		/// Per platform, the default input producers are:
		/// - on Windows: SDLInputProducer
		InputProducer* CreateDefaultProducer();

		/// Destructs a platform-dependent default InputProducer
		void DestructDefaultProducer(InputProducer* _producer);
	}
}