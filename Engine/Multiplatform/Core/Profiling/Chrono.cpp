#include "Profiling/Chrono.h"

#include <chrono>
#include <ratio>
#include <ctime>

std::chrono::high_resolution_clock::time_point Neptune::Chrono::Start()
{
	return std::chrono::high_resolution_clock::now();
}

double Neptune::Chrono::Stop(const std::chrono::high_resolution_clock::time_point& _startTime)
{
	std::chrono::high_resolution_clock::time_point t_now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t_now - _startTime);

	return time_span.count()*1000.0; // returns in ms
}