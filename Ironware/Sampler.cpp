/*!
 * \class Sampler
 *
 * \brief Sampler is responsible for performing lookups in a texture,
 * * such as setting filtering, wrapping mode etc.
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
#include "Sampler.h"
#include "GraphicsExceptionMacros.h"

Sampler::Sampler( Graphics& gfx )
{
	INFOMAN( gfx );

	D3D11_SAMPLER_DESC descSampler = {};
	descSampler.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	descSampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GFX_THROW_INFO( GetDevice( gfx )->CreateSamplerState( &descSampler, &pSampler ) );
}
