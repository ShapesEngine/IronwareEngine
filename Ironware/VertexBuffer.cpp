/*!
 * \file VertexBuffer.cpp
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#include "VertexBuffer.h"
#include "GraphicsExceptionMacros.h"

VertexBuffer::VertexBuffer( Graphics & gfx, const VertexByteBuffer & vbuff, UINT offset ) :
	VertexBuffer( gfx, L"?", vbuff, offset )
{}

VertexBuffer::VertexBuffer( Graphics& gfx, const std::wstring& tag, const VertexByteBuffer& vbuff, UINT offset ) :
	tag( tag ),
	stride( (UINT)layout.Size() ),
	offset( offset ),
	layout( vbuff.GetLayout() )
{
	INFOMAN( gfx );

	D3D11_BUFFER_DESC descVertexBuffer = {};
	descVertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	descVertexBuffer.Usage = D3D11_USAGE_DEFAULT;
	descVertexBuffer.CPUAccessFlags = 0u;
	descVertexBuffer.MiscFlags = 0u;
	descVertexBuffer.ByteWidth = UINT( vbuff.SizeBytes() );
	descVertexBuffer.StructureByteStride = stride;
	D3D11_SUBRESOURCE_DATA subresVertexData = {};
	subresVertexData.pSysMem = vbuff.GetData();
	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateBuffer( &descVertexBuffer, &subresVertexData, &pVertexBuffer ) );
}

std::shared_ptr<VertexBuffer> VertexBuffer::Resolve( Graphics& gfx, const std::wstring& tag, const VertexByteBuffer& vbuff, UINT offset )
{
	assert( tag != L"?" );
	return BindableCollection::Resolve<VertexBuffer>( gfx, tag, vbuff, offset );
}
