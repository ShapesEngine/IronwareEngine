/*!
 * \file CameraIndicator.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#pragma once

#include "Drawable.h"

class CameraIndicator : public Drawable
{
public:
	CameraIndicator( Graphics& gfx );
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

	void SetPos( DirectX::XMFLOAT3 pos_in ) noexcept { pos = pos_in; }
	void SetRotation( DirectX::XMFLOAT3 rot_in ) noexcept { rot = rot_in; }

private:
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rot = { 0.0f,0.0f,0.0f };
};

