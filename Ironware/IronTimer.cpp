/*!
 * \class IronTimer
 *
 * \brief A timer class that is responsible for timer events
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
#include "IronTimer.h"

using namespace std::chrono;

IronTimer::IronTimer() noexcept
{
	last = start = steady_clock::now();
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
	return duration<float>( steady_clock::now() - start ).count();
}
