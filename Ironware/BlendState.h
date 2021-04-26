/*!
 * \file BlendState.h
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#pragma once

#include "Bindable.h"
#include "BindableCollection.h"

 /**
  * @brief Responsible class for managing blending
 */
class BlendState : public Bindable
{
public:
	BlendState( Graphics& gfx, bool isBlending );
	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->OMSetBlendState( pBlendState.Get(), nullptr, 0xffffffffu ); }
	static std::shared_ptr<BlendState> Resolve( Graphics& gfx, bool isBlending ) { return BindableCollection::Resolve<BlendState>( gfx, isBlending ); }
	static std::wstring GenerateUID( bool isBlending ) { return GET_CLASS_WNAME( BlendState ) + L"#" + ( isBlending ? L"true" : L"false" ); }
	std::wstring GetUID() const noexcept override { return GenerateUID( isBlending ); }

private:
	Microsoft::WRL::ComPtr<ID3D11BlendState> pBlendState;
	bool isBlending;
};