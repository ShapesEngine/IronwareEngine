/*!
 * \file RenderTarget.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "GraphicsResource.h"
#include "DepthStencilView.h"

class RenderTarget : public GraphicsResource
{
public:
	RenderTarget( Graphics& gfx, UINT width, UINT height );
	void BindAsTexture( Graphics& gfx, UINT slot ) const noexcept;
	void BindAsTarget( Graphics& gfx ) const noexcept { GetContext( gfx )->OMSetRenderTargets( 1u, pTargetView.GetAddressOf(), nullptr ); }
	void BindAsTarget( Graphics& gfx, const class DepthStencilView& dsv ) { GetContext( gfx )->OMSetRenderTargets( 1u, pTargetView.GetAddressOf(), dsv.pDepthStencilView.Get() ); }

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
};