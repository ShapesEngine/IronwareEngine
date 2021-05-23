/*!
 * \file ShadowSampler.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "ShadowSampler.h"
#include "ConstantBuffers.h"

ShadowSampler::ShadowSampler( Graphics & gfx )
{
	INFOMAN( gfx );

	D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };

	samplerDesc.BorderColor[0] = 1.f;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateSamplerState( &samplerDesc, &pSampler ) );
}