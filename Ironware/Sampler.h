/*!
 * \file Sampler.h
 * \date 2020/11/06 14:20
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief This file contains a class Sampler.
 * * 
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "Bindable.h"

/*!
 * \class Sampler
 *
 * \brief A class that is is responsible for setting lookup configs.
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
class Sampler : public Bindable
{
public:
	Sampler( Graphics& gfx, UINT slot = 0u, D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_MODE texAddrMode = D3D11_TEXTURE_ADDRESS_WRAP );
	__forceinline void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetSamplers( slot, 1u, pSampler.GetAddressOf() ); }

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	const UINT slot;
};

