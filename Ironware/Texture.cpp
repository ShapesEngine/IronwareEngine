/*!
 * \class Texture
 *
 * \brief Texture class is responsible for creating & loading the
 * * texture to the GPU
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
#include "Texture.h"
#include "Surface.h"
#include "GraphicsExceptionMacros.h"

Texture::Texture( Graphics& gfx, const Surface& sur )
{
	INFOMAN( gfx );

	auto width = sur.GetWidth();
	auto height = sur.GetHeight();

	// =======================================================================
	// Create texture resource
	// -----------------------------------------------------------------------
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
	descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	descTexture.CPUAccessFlags = 0u;
	descTexture.MiscFlags = 0u;
	D3D11_SUBRESOURCE_DATA subresTextureData = {};
	subresTextureData.pSysMem = sur.GetBufferPtrConst();
	// row byte size === ( for ex. pixel[0][0] + width * sizeof( Surface::Color ) = pixel[1][0] )
	subresTextureData.SysMemPitch = width * sizeof( Surface::Color );
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	GFX_THROW_INFO( GetDevice( gfx )->CreateTexture2D( &descTexture, &subresTextureData, &pTexture ) );

	// =======================================================================
	// Create the resource view on the texture
	// -----------------------------------------------------------------------
	D3D11_SHADER_RESOURCE_VIEW_DESC descShaderResView = {};
	descShaderResView.Format = descTexture.Format;
	descShaderResView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descShaderResView.Texture2D.MostDetailedMip = 0u;
	descShaderResView.Texture2D.MipLevels = 1u;
	GFX_THROW_INFO( GetDevice( gfx )->CreateShaderResourceView( pTexture.Get(), &descShaderResView, &pTextureView ) );
}
