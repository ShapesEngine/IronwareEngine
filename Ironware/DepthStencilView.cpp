/*!
 * \file DepthStencilView.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "DepthStencilView.h"
#include "RenderTarget.h"
#include "GraphicsExceptionMacros.h"

#include <cassert>

namespace wrl = Microsoft::WRL;

DepthStencilView::DepthStencilView( Graphics & gfx, UINT width, UINT height, bool canBindShaderInput )
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

void DepthStencilView::BindAsBuffer( Graphics & gfx, BufferResource * renderTarget ) IFNOEXCEPT
{
	assert( dynamic_cast<RenderTarget*>( renderTarget ) != nullptr );
	BindAsBuffer( gfx, static_cast<RenderTarget*>( renderTarget ) );
}

ShaderInputDepthStencil::ShaderInputDepthStencil( Graphics & gfx, UINT slot ) :
	ShaderInputDepthStencil( gfx, gfx.GetWidth(), gfx.GetHeight(), slot )
{}

ShaderInputDepthStencil::ShaderInputDepthStencil( Graphics & gfx, UINT width, UINT height, UINT slot ) :
	DepthStencilView( gfx, width, height, true ),
	slot( slot )
{
	INFOMAN( gfx );

	wrl::ComPtr<ID3D11Resource> pRes;
	pDepthStencilView->GetResource( &pRes );

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // this will need to be fixed
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateShaderResourceView(
		pRes.Get(), &srvDesc, &pShaderResourceView
	) );
}

OutputOnlyDepthStencil::OutputOnlyDepthStencil( Graphics & gfx ) :
	OutputOnlyDepthStencil( gfx, gfx.GetWidth(), gfx.GetHeight() )
{}

OutputOnlyDepthStencil::OutputOnlyDepthStencil( Graphics & gfx, UINT width, UINT height ) :
	DepthStencilView( gfx, width, height, false )
{}

void OutputOnlyDepthStencil::Bind( Graphics & gfx ) IFNOEXCEPT
{
	assert( "OutputOnlyDepthStencil cannot be bound as shader input" && false );
}
