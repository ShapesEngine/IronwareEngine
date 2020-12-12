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
	void Reset() noexcept { pos = { 0.f, 0.f, 0.f }; }
	void Bind( Graphics& gfx ) const noexcept { cbuffer.Update( gfx,PointLightCBuf{ pos } ); }

private:
	struct PointLightCBuf
	{
		DirectX::XMFLOAT3 pos;
		// to make the struct's size 16 byte
		float padding;
	};

private:
	DirectX::XMFLOAT3 pos = { 0.f, 0.f, 0.f };
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuffer;
};

