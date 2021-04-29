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
	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->OMSetDepthStencilState( pDSState.Get(), 0xFFu ); }
	std::wstring GetUID() const noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	StencilMode mode;
};