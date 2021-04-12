/*!
 * \file PixelShader.cpp
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 *
 *
 */
#include "PixelShader.h"
#include "GraphicsExceptionMacros.h"

#include <d3dcompiler.h>

PixelShader::PixelShader( Graphics& gfx, const std::wstring& path ) :
	path( path )
{
	INFOMAN( gfx );

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_CALL_THROW_INFO( D3DReadFileToBlob( path.c_str(), &pBlob ) );
	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader ) );
}
