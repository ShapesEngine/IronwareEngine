/*!
 * \file Sampler.cpp
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 *
 *
 */
#include "Sampler.h"
#include "GraphicsExceptionMacros.h"

Sampler::Sampler( Graphics& gfx, UINT slot, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE texAddrMode ) :
	slot( slot )
{
	INFOMAN( gfx );

	D3D11_SAMPLER_DESC descSampler = {};
	descSampler.Filter = filter;
	descSampler.AddressU = texAddrMode;
	descSampler.AddressV = texAddrMode;
	descSampler.AddressW = texAddrMode;
	descSampler.MipLODBias = 0.f;
	descSampler.MinLOD = 0.f;
	descSampler.MaxLOD = D3D11_FLOAT32_MAX;
	descSampler.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateSamplerState( &descSampler, &pSampler ) );
}
