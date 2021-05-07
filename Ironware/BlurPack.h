/*!
 * \file BlurPack.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "BindableCommon.h"
#include "IronMath.h"

class BlurPack
{
public:
	BlurPack( Graphics& gfx, int radius = 7, float sigma = 2.6f ) :
		shader( gfx, L"Filter_PS.cso" ),
		pcb( gfx, 0u ),
		ccb( gfx, 1u )
	{
		SetKernel( gfx, radius, sigma );
	}
	void Bind( Graphics& gfx ) noexcept
	{
		shader.Bind( gfx );
		pcb.Bind( gfx );
		ccb.Bind( gfx );
	}
	void SetHorizontal( Graphics& gfx )
	{
		ccb.Update( gfx, { TRUE } );
	}
	void SetVertical( Graphics& gfx )
	{
		ccb.Update( gfx, { FALSE } );
	}
	void SetKernel( Graphics& gfx, int radius, float sigma ) IFNOEXCEPT
	{
		assert( radius <= 7 );
		Kernel k;
		k.nTaps = radius * 2 + 1;
		float sum = 0.0f;
		for( int i = 0; i < k.nTaps; i++ )
		{
			const auto x = float( i - radius );
			const auto g = gauss( x, sigma );
			sum += g;
			k.coefficients[i].x = g;
		}
		for( int i = 0; i < k.nTaps; i++ )
		{
			k.coefficients[i].x /= sum;
		}
		pcb.Update( gfx, k );
	}
private:
	struct Kernel
	{
		alignas( 16 ) int nTaps;
		DirectX::XMFLOAT4 coefficients[15];
	};

	struct Control
	{
		alignas( 16 ) BOOL horizontal;
	};

	PixelShader shader;
	PixelConstantBuffer<Kernel> pcb;
	PixelConstantBuffer<Control> ccb;
};