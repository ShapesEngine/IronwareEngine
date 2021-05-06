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
#include "BindableCollection.h"
#include "IronUtils.h"

#include <typeinfo>

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
	Sampler( Graphics& gfx, bool afEnabled, bool reflect, UINT slot = 0u );
	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetSamplers( slot, 1u, pSampler.GetAddressOf() ); }
	static std::shared_ptr<Sampler> Resolve( Graphics& gfx, bool afEnabled = true, bool reflect = false, UINT slot = 0u ) { return BindableCollection::Resolve<Sampler>( gfx, afEnabled, reflect, slot ); }
	static std::wstring GenerateUID( bool afEnabled, bool reflect, UINT slot ) { return GET_CLASS_WNAME( Sampler ) + ( afEnabled ? L"#AF" : L"#PT" ) + ( reflect ? L"#R" : L"#W" ) + L"#" + std::to_wstring( slot ); }
	std::wstring GetUID() const noexcept override { return GenerateUID( anisotropicEnabled, reflection, slot ); }

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	const UINT slot;
	bool anisotropicEnabled;
	bool reflection;
};