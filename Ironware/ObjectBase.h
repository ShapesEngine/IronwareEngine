/*!
 * \file ObjectBase.h
 * \date 2020/12/14 21:54
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief 
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "DrawableBase.h"

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
		r( rdist( rng ) ),
		droll( ddist( rng ) ),
		dpitch( ddist( rng ) ),
		dyaw( ddist( rng ) ),
		dphi( odist( rng ) ),
		dtheta( odist( rng ) ),
		dchi( odist( rng ) ),
		chi( adist( rng ) ),
		theta( adist( rng ) ),
		phi( adist( rng ) )
	{}

	void Update( float dt ) noexcept
	{
		roll += droll * dt;
		pitch += dpitch * dt;
		yaw += dyaw * dt;
		theta += dtheta * dt;
		phi += dphi * dt;
		chi += dchi * dt;
	}

	DirectX::XMMATRIX GetTransformXM() const noexcept
	{
		namespace dx = DirectX;
		return dx::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) *
			dx::XMMatrixTranslation( r, 0.f, 0.f ) *
			dx::XMMatrixRotationRollPitchYaw( theta, phi, chi );
	}

private:
	// positional
	float r;
	float roll = 0.f;
	float pitch = 0.f;
	float yaw = 0.f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};


