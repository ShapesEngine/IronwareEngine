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

namespace PipelineBindable
{
	Sampler::Sampler( Graphics& gfx, UINT slot, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE texAddrMode ) :
		slot( slot )
	{
		INFOMAN( gfx );

		D3D11_SAMPLER_DESC descSampler = {};
		descSampler.Filter = filter;
		descSampler.AddressU = texAddrMode;
		descSampler.AddressV = texAddrMode;
		descSampler.AddressW = texAddrMode;

		GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateSamplerState( &descSampler, &pSampler ) );
	}
}