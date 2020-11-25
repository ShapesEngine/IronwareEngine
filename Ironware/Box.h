/*!
 * \file Box.h
 * \date 2020/11/23 6:17
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A Box child class will control(partly) the graphics pipeline and draw the box
 *
 * TODO:
 *
 * \note It contains variables that are needed for translation, rotation, etc.
*/
#pragma once

#include "DrawableBase.h"

class Box : public DrawableBase<Box>
{
public:
	Box( Graphics& gfx, std::mt19937& rng,
		 std::uniform_real_distribution<float>& adist,
		 std::uniform_real_distribution<float>& ddist,
		 std::uniform_real_distribution<float>& odist,
		 std::uniform_real_distribution<float>& rdist );

	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
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

