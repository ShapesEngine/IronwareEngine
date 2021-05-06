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

Sampler::Sampler( Graphics & gfx, bool afEnabled, bool reflect, UINT slot ) :
	anisotropicEnabled( afEnabled ),
	reflection( reflect ),
	slot( slot )
{
	INFOMAN( gfx );

	D3D11_SAMPLER_DESC descSampler = {};
	descSampler.Filter = anisotropicEnabled ? D3D11_FILTER_ANISOTROPIC : D3D11_FILTER_MIN_MAG_MIP_POINT;
	descSampler.AddressU = reflection ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.AddressV = reflection ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.AddressW = reflection ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.MipLODBias = 0.f;
	descSampler.MinLOD = 0.f;
	descSampler.MaxLOD = D3D11_FLOAT32_MAX;
	descSampler.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateSamplerState( &descSampler, &pSampler ) );
}