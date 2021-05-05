/*!
 * \file ConstantBuffersEx.cpp
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 * 
 */
#include "ConstantBuffersEx.h"

void ConstantBufferEx::Update( Graphics & gfx, const Buffer & buf )
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

std::wstring ConstantBufferEx::GetUID() const noexcept
{
	assert( false && "Not yet integrated with bCollection system!" );
	return L"?";
}

ConstantBufferEx::ConstantBufferEx( Graphics & gfx, const LayoutElement & layoutRoot, UINT slot, const Buffer * pBuf ) :
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