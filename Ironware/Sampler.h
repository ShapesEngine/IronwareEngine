/*!
 * \file Sampler.h
 * \date 2020/11/06 14:20
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief This file contains a (bindable) sampler class.
 *
*/
#pragma once

#include "Bindable.h"

namespace PipelineBindable
{
	/*!
	 * \class Sampler
	 *
	 * \ingroup Bindables
	 *
	 * \brief A class that is is responsible for setting lookup configs.
	 *
	 * \author Yernar Aldabergenov
	 *
	 * \date September 2020
	 *
	 * Contact: yernar.aa@gmail.com
	 *
	 */
	class Sampler : public Bindable
	{
	public:
		Sampler( Graphics& gfx, UINT slot = 0u, D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_MODE texAddrMode = D3D11_TEXTURE_ADDRESS_WRAP );
		void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetSamplers( slot, 1u, pSampler.GetAddressOf() ); }

	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
		const UINT slot;
	};
}