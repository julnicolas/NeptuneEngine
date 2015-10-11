#pragma once

namespace Neptune
{
	///
	/// Enables a class to be updated on each frame.
	///
	class Updatable
	{
	public:
		virtual bool init() =0;        /// Prepares the object to be updated.
		virtual bool update() =0;      /// Executed once per frame.
		virtual void terminate() =0;   /// Executed once the object gets out of the update loop.
	};

}