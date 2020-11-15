/*!
 * \file IronTimer.h
 * \date 2020/11/14 13:22
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A timer class that is responsible for timer events
 *
 * TODO:
 *
 * \note Use Mark function to get delta time from the last time the Mark function was called
 * * and Peek function will not reset the mark point
*/
#pragma once

#include <chrono>

class IronTimer
{
public:
	IronTimer() noexcept;
	float Mark() noexcept;
	float Peek() const noexcept;
private:
	std::chrono::steady_clock::time_point last;
};