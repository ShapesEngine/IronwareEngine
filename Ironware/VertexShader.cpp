/*!
 * \file VertexShader.cpp
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 *
 * 
 */
#include "VertexShader.h"
#include "GraphicsExceptionMacros.h"

VertexShader::VertexShader( Graphics& gfx, const std::wstring& path )
{
	INFOMAN( gfx );

	GFX_CALL_THROW_INFO( D3DReadFileToBlob( path.c_str(), &pBytecodeBlob ) );
	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateVertexShader(
		pBytecodeBlob->GetBufferPointer(),
		pBytecodeBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	) );
}
