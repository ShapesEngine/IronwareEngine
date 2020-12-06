#pragma once

#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler( Graphics& gfx );
	inline void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetSamplers( 0u, 1u, pSampler.GetAddressOf() ); }

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
};

