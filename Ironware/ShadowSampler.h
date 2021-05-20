/*!
 * \file ShadowSampler.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "Bindable.h"

class ShadowSampler : public Bindable
{
public:
	ShadowSampler( Graphics& gfx );
	void Bind( Graphics& gfx ) IFNOEXCEPT override { GetContext( gfx )->PSSetSamplers( 1u, 1u, pSampler.GetAddressOf() ); };

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
};