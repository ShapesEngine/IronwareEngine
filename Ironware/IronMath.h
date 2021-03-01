/*!
 * \file IronMath.h
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 *
 * 
 */
#pragma once

#include <cmath>

constexpr double PI_D = 3.1415926535897932;
constexpr float PI = float( PI_D );

template <typename T>
constexpr auto sq( const T& x )
{
	return x * x;
}

template<typename T>
T wrap_angle( T theta )
{
	const T modded = fmod( theta, (T)2.0 * (T)PI_D );
	return ( modded > (T)PI_D ) ?
		( modded - (T)2.0 * (T)PI_D ) :
		modded;
}

template<typename T>
constexpr T interpolate( const T& src, const T& dst, float alpha )
{
	return src + ( dst - src ) * alpha;
}

template<typename T>
constexpr T to_rad( T deg )
{
	return deg * PI / (T)180.0;
}