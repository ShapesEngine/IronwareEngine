/*!
 * \file Texture.cpp
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 *
 *
 */
#include "Texture.h"
#include "Surface.h"
#include "GraphicsExceptionMacros.h"

Texture::Texture( Graphics& gfx, const std::wstring& path, UINT slot ) :
	path( path ),
	slot( slot )
{
	INFOMAN( gfx );

	const auto sur = Surface::FromFile( path );
	hasAlpha = sur.IsAlphaLoaded();
	auto width = sur.GetWidth();
	auto height = sur.GetHeight();

	// =======================================================================
	// Create texture resource
	// -----------------------------------------------------------------------
	D3D11_TEXTURE2D_DESC descTexture = {};
	descTexture.Width = width;
	descTexture.Height = height;
	descTexture.MipLevels = 0u;
	descTexture.ArraySize = 1u;
	descTexture.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	// multi sampling parameters for a resource
	descTexture.SampleDesc.Count = 1u;
	descTexture.SampleDesc.Quality = 0u;
	descTexture.Usage = D3D11_USAGE_DEFAULT;
	// enabling input and output merger stage
	descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	descTexture.CPUAccessFlags = 0u;
	descTexture.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateTexture2D( &descTexture, nullptr, &pTexture ) );

	GetContext( gfx )->UpdateSubresource( pTexture.Get(), 0u, nullptr, sur.GetBufferPtrConst(), width * sizeof( Surface::Color ), 0u );

	// =======================================================================
	// Create the resource view on the texture
	// -----------------------------------------------------------------------
	D3D11_SHADER_RESOURCE_VIEW_DESC descShaderResView = {};
	descShaderResView.Format = descTexture.Format;
	descShaderResView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descShaderResView.Texture2D.MostDetailedMip = 0u;
	descShaderResView.Texture2D.MipLevels = -1;
	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateShaderResourceView( pTexture.Get(), &descShaderResView, &pTextureView ) );

	GFX_CALL_THROW_INFO_ONLY( GetContext( gfx )->GenerateMips( pTextureView.Get() ) );
}