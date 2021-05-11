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
#include <DirectXMath.h>

constexpr double PI_D = 3.1415926535897932;
constexpr float PI = float( PI_D );

template <typename T>
constexpr auto sq( const T& x ) noexcept
{
	return x * x;
}

/**
 * @brief Wraps an angle to the range between -PI and +PI
 * @tparam T type of the angle
 * @param theta angle in radians
 * @return wrapped angle in radians
*/
template<typename T>
T wrap_angle( T theta ) noexcept
{
	constexpr T twoPi = (T)2 * (T)PI_D;
	const T mod = fmod( theta, twoPi );
	if( mod > (T)PI_D )
	{
		return mod - twoPi;
	}
	if( mod < -(T)PI_D )
	{
		return mod + twoPi;
	}
	return mod;
}

template<typename T>
constexpr T interpolate( const T& src, const T& dst, float alpha ) noexcept
{
	return src + ( dst - src ) * alpha;
}

template<typename T>
constexpr T to_rad( T deg ) noexcept
{
	return deg * PI / (T)180.0;
}

template<typename T>
constexpr T gauss(T x, T sigma) noexcept
{
	const auto sigsq = sq( sigma );
	return ( (T)1.0 / sqrt( (T)2.0 * (T)PI_D * sigsq ) ) * exp( -sq( x ) / ( (T)2.0 * sigsq ) );
}

DirectX::XMFLOAT3 extract_euler_angles( const DirectX::XMFLOAT4X4& matrix ) noexcept;

DirectX::XMFLOAT3 extract_translation( const DirectX::XMFLOAT4X4& matrix ) noexcept;

DirectX::XMMATRIX scale_translation( DirectX::FXMMATRIX matrix, float scale ) noexcept;