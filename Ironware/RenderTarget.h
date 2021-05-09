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

#include <array>

class RenderTarget : public GraphicsResource
{
public:
	RenderTarget( Graphics& gfx, UINT width, UINT height );
	void SetViewport( Graphics& gfx ) const;

	void BindAsTexture( Graphics& gfx, UINT slot ) const noexcept { GetContext( gfx )->PSSetShaderResources( slot, 1u, pTextureView.GetAddressOf() ); }
	void BindAsTarget( Graphics& gfx ) const noexcept;
	void BindAsTarget( Graphics& gfx, const DepthStencilView& dsv ) const noexcept;
	void Clear( Graphics& gfx, const std::array<float, 4>& color ) const noexcept { GetContext( gfx )->ClearRenderTargetView( pTargetView.Get(), color.data() ); }
	void Clear( Graphics& gfx ) const noexcept { Clear( gfx, { 0.f, 0.f, 0.f, 0.f } ); }

private:
	UINT width;
	UINT height;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
};