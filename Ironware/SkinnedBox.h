/*!
 * \file SkinnedBox.h
 * \date 2020/11/06 15:33
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A SkinnedBox child class will control(partly) the graphics pipeline and draw the skinned box
 *
 * TODO:
 *
 * \note It contains variables that are needed for translation, rotation, etc.
*/
#pragma once

#include "DrawableBase.h"

class SkinnedBox : public DrawableBase<SkinnedBox>
{
public:
	SkinnedBox( Graphics& gfx, std::mt19937& rng,
				std::uniform_real_distribution<float>& adist,
				std::uniform_real_distribution<float>& ddist,
				std::uniform_real_distribution<float>& odist,
				std::uniform_real_distribution<float>& rdist );
	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	// positional
	float zOffset;
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