/*!
 * \file RenderTarget.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "RenderTarget.h"
#include "GraphicsExceptionMacros.h"

namespace wrl = Microsoft::WRL;

RenderTarget::RenderTarget( Graphics & gfx, UINT width, UINT height )
{
	INFOMAN( gfx );

	D3D11_TEXTURE2D_DESC descTexture = {};
	descTexture.Width = width;
	descTexture.Height = height;
	descTexture.MipLevels = 1u;
	descTexture.ArraySize = 1u;
	descTexture.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	// multi sampling parameters for a resource
	descTexture.SampleDesc.Count = 1u;
	descTexture.SampleDesc.Quality = 0u;
	descTexture.Usage = D3D11_USAGE_DEFAULT;
	// enabling input and output merger stage
	descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	descTexture.CPUAccessFlags = 0u;
	descTexture.MiscFlags = 0u;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateTexture2D( &descTexture, nullptr, &pTexture ) );

	D3D11_SHADER_RESOURCE_VIEW_DESC descShaderResView = {};
	descShaderResView.Format = descTexture.Format;
	descShaderResView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descShaderResView.Texture2D.MostDetailedMip = 0u;
	descShaderResView.Texture2D.MipLevels = 1u;
	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateShaderResourceView( pTexture.Get(), &descShaderResView, &pTextureView ) );

	D3D11_RENDER_TARGET_VIEW_DESC descRenderTar = {};
	descRenderTar.Format = descTexture.Format;
	descRenderTar.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	descRenderTar.Texture2D = D3D11_TEX2D_RTV{ 0 };
	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateRenderTargetView(
		pTexture.Get(), &descRenderTar, &pTargetView
	) );
}