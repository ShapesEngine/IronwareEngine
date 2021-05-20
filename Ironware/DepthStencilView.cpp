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
#include "SurfaceEx.h"

#include <cassert>
#include <stdexcept>

namespace wrl = Microsoft::WRL;

DXGI_FORMAT MapUsageTypeless( DepthStencilView::Usage usage )
{
	switch( usage )
	{
	case DepthStencilView::Usage::DepthStencilView:
		return DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
	case DepthStencilView::Usage::ShadowDepth:
		return DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	}
	throw std::runtime_error{ "Base usage for Typeless format map in DepthStencilView." };
}

DXGI_FORMAT MapUsageTyped( DepthStencilView::Usage usage )
{
	switch( usage )
	{
	case DepthStencilView::Usage::DepthStencilView:
		return DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	case DepthStencilView::Usage::ShadowDepth:
		return DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	}
	throw std::runtime_error{ "Base usage for Typed format map in DepthStencilView." };
}

DXGI_FORMAT MapUsageColored( DepthStencilView::Usage usage )
{
	switch( usage )
	{
	case DepthStencilView::Usage::DepthStencilView:
		return DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	case DepthStencilView::Usage::ShadowDepth:
		return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	}
	throw std::runtime_error{ "Base usage for Colored format map in DepthStencilView." };
}

DepthStencilView::DepthStencilView( Graphics & gfx, UINT width, UINT height, bool canBindShaderInput, Usage usage ) :
	width( width ),
	height( height )
{
	INFOMAN( gfx );

	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = MapUsageTypeless( usage );
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | ( canBindShaderInput ? D3D11_BIND_SHADER_RESOURCE : 0u );
	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateTexture2D( &descDepth, nullptr, &pDepthStencil ) );

	// create target view of depth stencil view texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descView = {};
	descView.Format = MapUsageTyped( usage );
	descView.Flags = 0u;
	descView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descView.Texture2D.MipSlice = 0u;

	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateDepthStencilView(
		pDepthStencil.Get(), &descView, &pDepthStencilView
	) );
}

void DepthStencilView::BindAsBuffer( Graphics & gfx, BufferResource * renderTarget ) IFNOEXCEPT
{
	assert( dynamic_cast<RenderTarget*>( renderTarget ) != nullptr );
	BindAsBuffer( gfx, static_cast<RenderTarget*>( renderTarget ) );
}

SurfaceEx DepthStencilView::ToSurface( Graphics & gfx, bool toLinearize ) const
{
	INFOMAN( gfx );
	namespace wrl = Microsoft::WRL;

	// creating a temp texture compatible with the source, but with CPU read access
	wrl::ComPtr<ID3D11Resource> pResSource;
	pDepthStencilView->GetResource( &pResSource );
	wrl::ComPtr<ID3D11Texture2D> pTexSource;
	pResSource.As( &pTexSource );
	D3D11_TEXTURE2D_DESC textureDesc;
	pTexSource->GetDesc( &textureDesc );
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textureDesc.Usage = D3D11_USAGE_STAGING;
	textureDesc.BindFlags = 0;
	wrl::ComPtr<ID3D11Texture2D> pTexTemp;
	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateTexture2D(
		&textureDesc, nullptr, &pTexTemp
	) );

	// copy texture contents
	GFX_CALL_THROW_INFO_ONLY( GetContext( gfx )->CopyResource( pTexTemp.Get(), pTexSource.Get() ) );

	// create Surface and copy from temp texture to it
	const auto width = GetWidth();
	const auto height = GetHeight();
	SurfaceEx s{ width, height };
	D3D11_MAPPED_SUBRESOURCE msr = {};
	GFX_CALL_THROW_INFO( GetContext( gfx )->Map( pTexTemp.Get(), 0, D3D11_MAP::D3D11_MAP_READ, 0, &msr ) );
	auto pSrcBytes = static_cast<const char*>( msr.pData );
	for( uint32_t y = 0; y < height; y++ )
	{
		struct Pixel
		{
			char data[4];
		};
		auto pSrcRow = reinterpret_cast<const Pixel*>( pSrcBytes + msr.RowPitch * size_t( y ) );
		for( uint32_t x = 0; x < width; x++ )
		{
			if( textureDesc.Format == DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS )
			{
				const auto raw = 0xFFFFFF & *reinterpret_cast<const uint32_t*>( pSrcRow + x );
				if( toLinearize )
				{
					const auto normalized = (float)raw / (float)0xFFFFFF;
					const auto linearized = 0.01f / ( 1.01f - normalized );
					const uint8_t channel = uint8_t( linearized * 255.f );
					s.PutPixel( x, y, { channel, channel, channel } );
				}
				else
				{
					const uint8_t channel = raw >> 16;
					s.PutPixel( x, y, { channel, channel, channel } );
				}

			}
			else if( textureDesc.Format == DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS )
			{
				const auto raw = *reinterpret_cast<const float*>( pSrcRow + x );
				if( toLinearize )
				{
					const auto linearized = 0.01f / ( 1.01f - raw );
					const auto channel = uint8_t( linearized * 255.f );
					s.PutPixel( x, y, { channel, channel, channel } );
				}
				else
				{
					const auto channel = uint8_t( raw * 255.f );
					s.PutPixel( x, y, { channel,channel,channel } );
				}

			}
			else
			{
				throw std::runtime_error{ "Bad format in Depth Stencil for conversion to Surface" };
			}
		}
	}
	GFX_CALL_THROW_INFO_ONLY( GetContext( gfx )->Unmap( pTexTemp.Get(), 0u ) );

	return s;
}

ShaderInputDepthStencil::ShaderInputDepthStencil( Graphics & gfx, UINT slot, Usage usage ) :
	ShaderInputDepthStencil( gfx, gfx.GetWidth(), gfx.GetHeight(), slot )
{}

ShaderInputDepthStencil::ShaderInputDepthStencil( Graphics & gfx, UINT width, UINT height, UINT slot, Usage usage ) :
	DepthStencilView( gfx, width, height, true, usage ),
	slot( slot )
{
	INFOMAN( gfx );

	wrl::ComPtr<ID3D11Resource> pRes;
	pDepthStencilView->GetResource( &pRes );

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = MapUsageColored( usage );
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0u;
	srvDesc.Texture2D.MipLevels = 1u;
	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateShaderResourceView(
		pRes.Get(), &srvDesc, &pShaderResourceView
	) );
}

OutputOnlyDepthStencil::OutputOnlyDepthStencil( Graphics & gfx ) :
	OutputOnlyDepthStencil( gfx, gfx.GetWidth(), gfx.GetHeight() )
{}

OutputOnlyDepthStencil::OutputOnlyDepthStencil( Graphics & gfx, UINT width, UINT height ) :
	DepthStencilView( gfx, width, height, false, DepthStencilView::Usage::DepthStencilView )
{}

void OutputOnlyDepthStencil::Bind( Graphics & gfx ) IFNOEXCEPT
{
	assert( "OutputOnlyDepthStencil cannot be bound as shader input" && false );
}
