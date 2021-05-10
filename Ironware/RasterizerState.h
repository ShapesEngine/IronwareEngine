/*!
 * \file RasterizerState.h
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
 * @brief Responsible class for controlling rasterizer state
*/
class RasterizerState : public Bindable
{
public:
	RasterizerState( Graphics& gfx, bool isTwoSided );
	void Bind( Graphics& gfx ) IFNOEXCEPT override { GetContext( gfx )->RSSetState( pRasterizerState.Get() ); }
	static std::shared_ptr<RasterizerState> Resolve( Graphics& gfx, bool isTwoSided ) { return BindableCollection::Resolve<RasterizerState>( gfx, isTwoSided ); }
	static std::wstring GenerateUID( bool isBlending ) { return GET_CLASS_WNAME( RasterizerState ) + L"#" + ( isBlending ? L"true" : L"false" ); }
	std::wstring GetUID() const noexcept override { return GenerateUID( isTwoSided ); }

private:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizerState;
	bool isTwoSided;
};

