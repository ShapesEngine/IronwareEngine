/*!
 * \file ConstantBuffersEx.cpp
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 * 
 */
#include "ConstantBuffersEx.h"

void PixelConstantBufferEx::Update( Graphics & gfx, const Buffer & buf )
{
	assert( &buf.GetRootLayoutElement() == &GetRootLayoutElement() );
	INFOMAN( gfx );

	D3D11_MAPPED_SUBRESOURCE msr;
	GFX_CALL_THROW_INFO( GetContext( gfx )->Map(
		pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u,
		&msr
	) );
	memcpy( msr.pData, buf.GetData(), buf.GetSizeInBytes() );
	GetContext( gfx )->Unmap( pConstantBuffer.Get(), 0u );
}

std::wstring PixelConstantBufferEx::GetUID() const noexcept
{
	assert( false && "Not yet integrated with bCollection system!" );
	return L"?";
}

PixelConstantBufferEx::PixelConstantBufferEx( Graphics & gfx, const LayoutElement & layoutRoot, UINT slot, const Buffer * pBuf ) :
	slot( slot )
{
	INFOMAN( gfx );

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = (UINT)layoutRoot.GetSizeInBytes();
	cbd.StructureByteStride = 0u;

	if( pBuf != nullptr )
	{
		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = pBuf->GetData();
		GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateBuffer( &cbd, &csd, &pConstantBuffer ) );
	}
	else
	{
		GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateBuffer( &cbd, nullptr, &pConstantBuffer ) );
	}
}

CachingPixelConstantBufferEx::CachingPixelConstantBufferEx( Graphics & gfx, const CookedLayout & layout, UINT slot ) :
	PixelConstantBufferEx( gfx, *layout.ShareRoot(), slot, nullptr ),
	buf( Buffer( layout ) )
{}

CachingPixelConstantBufferEx::CachingPixelConstantBufferEx( Graphics & gfx, const Buffer & buf, UINT slot ) :
	PixelConstantBufferEx( gfx, buf.GetRootLayoutElement(), slot, &buf ),
	buf( buf )
{}

void CachingPixelConstantBufferEx::SetBuffer( const Buffer & buf_in )
{
	buf.CopyFrom( buf_in );
	dirty = true;
}

void CachingPixelConstantBufferEx::Bind( Graphics & gfx ) noexcept
{
	if( dirty )
	{
		Update( gfx, buf );
		dirty = false;
	}
	PixelConstantBufferEx::Bind( gfx );
}

void CachingPixelConstantBufferEx::Accept( TechniqueProbe & probe )
{
	if( probe.VisitBuffer( buf ) )
	{
		dirty = true;
	}
}

NocachePixelConstantBufferEx::NocachePixelConstantBufferEx( Graphics & gfx, const CookedLayout & layout, UINT slot ) :
	PixelConstantBufferEx( gfx, *layout.ShareRoot(), slot, nullptr ),
	pLayoutRoot( layout.ShareRoot() )
{}

NocachePixelConstantBufferEx::NocachePixelConstantBufferEx( Graphics & gfx, const Buffer & buf, UINT slot ) :
	PixelConstantBufferEx( gfx, buf.GetRootLayoutElement(), slot, &buf ),
	pLayoutRoot( buf.ShareLayoutRoot() )
{}