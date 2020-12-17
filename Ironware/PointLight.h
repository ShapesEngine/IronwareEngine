/*!
 * \file PointLight.h
 * \date 2020/12/11 5:09
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A class that represents point light that can be set into the scene
 *
 * TODO:
 *
 * \note It uses sphere as its mesh but you can change by changing some code
*/
#pragma once

#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"

class PointLight
{
public:
	PointLight( Graphics& gfx, float radius = 0.5f );
	void SpawnControlWindow() noexcept;
	void Draw( Graphics& gfx ) const noexcept( !IS_DEBUG );
	void Bind( Graphics& gfx, DirectX::FXMMATRIX view ) const noexcept;
	void Reset() noexcept;

private:
	struct PointLightCBuf
	{
		// GPU expects 16 byte val
		alignas( 16 ) DirectX::XMFLOAT3 pos;
		alignas( 16 ) DirectX::XMFLOAT3 ambient;
		alignas( 16 ) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};

private:
	PointLightCBuf cbufData;
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuffer;
};

