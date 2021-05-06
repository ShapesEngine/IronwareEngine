/*!
 * \file DepthStencilView.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "GraphicsResource.h"

class DepthStencilView : public GraphicsResource
{
	friend class RenderTarget;
	friend Graphics;
public:
	DepthStencilView( Graphics& gfx, UINT width, UINT height );
	void BindAsDepthStencil( Graphics& gfx ) const noexcept { GetContext( gfx )->OMSetRenderTargets( 0u, nullptr, pDepthStencilView.Get() ); }
	void Clear( Graphics& gfx ) const noexcept { GetContext( gfx )->ClearDepthStencilView( pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0u ); }

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
};