/*!
 * \file PointLight.h
 * \date 2020/12/11 5:09
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

#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"

class PointLight
{
public:
	PointLight( Graphics& gfx, float radius = 0.5f );
	void SpawnControlWindow() noexcept;	
	void Draw( Graphics& gfx ) const noexcept( !IS_DEBUG );
	void Reset() noexcept;	
	void Bind( Graphics& gfx, DirectX::FXMMATRIX view ) const noexcept;

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

