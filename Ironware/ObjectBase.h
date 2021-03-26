/*!
 * \file ObjectBase.h
 * \date 2020/11/14 21:54
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A CRTP class for replacing boilerplate code in drawables
 *
*/
#pragma once

#include "DrawableBase.h"
#include "IronMath.h"

/*!
 * \class ObjectBase
 *
 * \ingroup Drawables
 *
 * \brief CRTP Class that is responsible for pasting boilerplate code
 *
 * \author Yernar Aldabergenov
 *
 * \date Novemeber 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
template<class T>
class ObjectBase : public DrawableBase<T>
{
public:
	ObjectBase( Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist )
		:
		zOffset( rdist( rng ) ),
		droll( ddist( rng ) ),
		dpitch( ddist( rng ) ),
		dyaw( ddist( rng ) ),
		dbeta( odist( rng ) ),
		dalpha( odist( rng ) ),
		dgamma( odist( rng ) ),
		gamma( adist( rng ) ),
		alpha( adist( rng ) ),
		beta( adist( rng ) )
	{}

	void Update( float dt ) noexcept
	{
		roll = wrap_angle( roll + droll * dt );
		pitch = wrap_angle( pitch + dpitch * dt );
		yaw = wrap_angle( yaw + dyaw * dt );
		alpha = wrap_angle( alpha + dalpha * dt );
		beta = wrap_angle( beta + dbeta * dt );
		gamma = wrap_angle( gamma + dgamma * dt );
	}

	DirectX::XMMATRIX GetTransformXM() const noexcept override
	{
		namespace dx = DirectX;
		return dx::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) *
			dx::XMMatrixTranslation( zOffset, 0.f, 0.f ) *
			dx::XMMatrixRotationRollPitchYaw( alpha, beta, gamma );
	}

protected:
	// positional
	float zOffset;
	// relative rotation
	float roll = 0.f;
	float pitch = 0.f;
	float yaw = 0.f;
	// world rotation
	float alpha;
	float beta;
	float gamma;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dalpha;
	float dbeta;
	float dgamma;
};