/*!
 * \file DepthStencilView.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "DepthStencilView.h"
#include "GraphicsExceptionMacros.h"

namespace wrl = Microsoft::WRL;

DepthStencilView::DepthStencilView( Graphics & gfx, UINT width, UINT height )
{
	INFOMAN( gfx );

	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateTexture2D( &descDepth, nullptr, &pDepthStencil ) );

	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateDepthStencilView(
		pDepthStencil.Get(), nullptr, &pDepthStencilView
	) );
}