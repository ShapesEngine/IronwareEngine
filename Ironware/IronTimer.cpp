#include "IronTimer.h"

using namespace std::chrono;

IronTimer::IronTimer() noexcept
{
	last = steady_clock::now();
}

float IronTimer::Mark() noexcept
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float IronTimer::Peek() const noexcept
{
	return duration<float>( steady_clock::now() - last ).count();
}
