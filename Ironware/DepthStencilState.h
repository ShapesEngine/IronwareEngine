/*!
 * \file DepthStencilState.h
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#pragma once

#include "Bindable.h"
#include "BindableCollection.h"

class DepthStencilState : public Bindable
{
public:
	enum class StencilMode
	{
		Off,
		Write,
		Mask
	};

public:
	DepthStencilState( Graphics& gfx, StencilMode mode );
	void Bind( Graphics& gfx ) IFNOEXCEPT override { GetContext( gfx )->OMSetDepthStencilState( pDSState.Get(), 0xFFu ); }
	static std::shared_ptr<DepthStencilState> Resolve( Graphics& gfx, StencilMode mode ) { return BindableCollection::Resolve<DepthStencilState>( gfx, mode ); }
	static std::wstring GenerateUID( StencilMode mode ) { return GET_CLASS_WNAME( DepthStencilState ) + L"#" + ( mode == StencilMode::Mask ? L"M" : mode == StencilMode::Write ? L"W" : L"O" ); }
	std::wstring GetUID() const noexcept override { return GenerateUID( mode ); }

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	StencilMode mode;
};