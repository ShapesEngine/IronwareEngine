/*!
 * \file Sampler.cpp
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 *
 *
 */
#include "Sampler.h"
#include "GraphicsExceptionMacros.h"

Sampler::Sampler( Graphics & gfx, Type type, bool reflect, UINT slot ) :
	type( type ),
	reflection( reflect ),
	slot( slot )
{
	INFOMAN( gfx );

	D3D11_SAMPLER_DESC descSampler = {};
	descSampler.Filter = [type]() {
		switch( type )
		{
		case Type::Anisotropic: return D3D11_FILTER_ANISOTROPIC;
		case Type::Point: return D3D11_FILTER_MIN_MAG_MIP_POINT;
		default:
		case Type::Bilinear: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		}
	}( );
	descSampler.AddressU = reflection ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.AddressV = reflection ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.AddressW = reflection ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.MipLODBias = 0.f;
	descSampler.MinLOD = 0.f;
	descSampler.MaxLOD = D3D11_FLOAT32_MAX;
	descSampler.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateSamplerState( &descSampler, &pSampler ) );
}

void Sampler::Bind( Graphics & gfx ) IFNOEXCEPT
{
	INFOMAN_NOHR( gfx );
	GetContext( gfx )->PSSetSamplers( slot, 1u, pSampler.GetAddressOf() );
}

std::wstring Sampler::GenerateUID( Type type, bool reflect, UINT slot )
{
	return GET_CLASS_WNAME( Sampler ) + L"#" + std::to_wstring( (int)type ) + ( reflect ? L"#R" : L"#W" ) + L"#" + std::to_wstring( slot );
}
