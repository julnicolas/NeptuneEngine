#pragma once

namespace Neptune
{
	///
	/// Enables a class to be updated on each frame.
	///
	class Updatable
	{
	public:
		/// \brief		Prepares object to be updated.
		virtual bool init()						= 0;

		/// Updates		object's data and performs its update task. Generally executed once per-frame.
		virtual bool update()					= 0;

		/// \brief		Frees update-related resources.
		/// \note		Clones must call this method once for them all because they share the same data to update().
		virtual void terminate()				= 0;   
	};

}