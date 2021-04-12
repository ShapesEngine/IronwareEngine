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
#include "IronUtils.h"
#include "BindableCollection.h"

#include <typeinfo>

VertexShader::VertexShader( Graphics& gfx, const std::wstring& path ) :
	path( path )
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

std::shared_ptr<Bindable> VertexShader::Resolve( Graphics& gfx, const std::wstring& path ) noexcept
{
	auto b = BindableCollection::Resolve( path );
	if( !b )
	{
		b = std::make_shared<VertexShader>( gfx, path );
		BindableCollection::Store( b );
	}

	return b;
}