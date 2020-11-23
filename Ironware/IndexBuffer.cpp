/*!
 * \class IndexBuffer
 *
 * \brief 
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
#include "IndexBuffer.h"
#include "GraphicsExceptionMacros.h"

IndexBuffer::IndexBuffer( Graphics& gfx, const std::vector<uint16_t>& indices ) :
	count( (UINT)indices.size() )
{
	INFOMAN( gfx );

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT( count * sizeof( uint16_t ) );
	ibd.StructureByteStride = sizeof( uint16_t );
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();
	GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &ibd, &isd, &pIndexBuffer ) );
}
