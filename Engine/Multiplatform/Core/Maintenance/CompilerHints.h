#pragma once

#ifdef NEP_COMPILER_VISUAL_STUDIO
	#define NEP_DEPRECATED(_message) __declspec(deprecated(_message))
#else
	#define NEP_DEPRECATED(_message)
#endif