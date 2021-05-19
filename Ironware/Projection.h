/*!
 * \file Projection.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "FrustumIndicator.h"

#include <DirectXMath.h>

class Graphics;
class RenderGraph;

class Projection
{
public:
	Projection( Graphics& gfx, float width, float height, float nearZ, float farZ );
	void RenderWidgets( Graphics& gfx );
	void SetPos( DirectX::XMFLOAT3 pos_in ) { indicator.SetPos( pos_in ); }
	void SetRotation( DirectX::XMFLOAT3 rot_in ) { indicator.SetRotation( rot_in ); }
	void Submit( size_t channelFilter ) const;
	void LinkTechniques( RenderGraph& rg );
	void Reset( Graphics& gfx );

	DirectX::XMMATRIX GetMatrix() const { return DirectX::XMMatrixPerspectiveLH( width, height, nearZ, farZ ); }

private:
	float width;
	float height;
	float nearZ;
	float farZ;
	float homeWidth;
	float homeHeight;
	float homeNearZ;
	float homeFarZ;
	FrustumIndicator indicator;
};