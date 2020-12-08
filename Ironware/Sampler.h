/*!
 * \file Sampler.h
 * \date 2020/12/06 14:20
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief This file contains a class Sampler.
 * * Sampler is responsible for performing lookups in a texture,
 * * such as setting filtering, wrapping etc.
 *
 * TODO:
 *
 * \note
*/
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

