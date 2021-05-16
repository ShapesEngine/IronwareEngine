/*!
 * \file Projection.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#pragma once

#include <DirectXMath.h>

class Projection
{
public:
	Projection( float width, float height, float nearZ, float farZ );
	void RenderWidgets();

	DirectX::XMMATRIX GetMatrix() const { return DirectX::XMMatrixPerspectiveLH( width, height, nearZ, farZ ); }

private:
	float width;
	float height;
	float nearZ;
	float farZ;
};