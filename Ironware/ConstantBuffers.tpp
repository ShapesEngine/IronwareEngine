/*!
 * \file ConstantBuffers.tpp
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 *
 * 
 */
template<typename C>
ConstantBuffer<C>::ConstantBuffer( Graphics& gfx, UINT slot ) :
	slot( slot )
{
	INFOMAN( gfx );

	D3D11_BUFFER_DESC descConstBuffer;
	descConstBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	descConstBuffer.Usage = D3D11_USAGE_DYNAMIC;
	descConstBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	descConstBuffer.MiscFlags = 0u;
	descConstBuffer.ByteWidth = sizeof( C );
	descConstBuffer.StructureByteStride = 0u;
	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateBuffer( &descConstBuffer, nullptr, &pConstantBuffer ) );
}

template<typename C>
ConstantBuffer<C>::ConstantBuffer( Graphics& gfx, const C& consts, UINT slot ) :
	slot( slot )
{
	INFOMAN( gfx );

	D3D11_BUFFER_DESC descConstBuffer;
	descConstBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	descConstBuffer.Usage = D3D11_USAGE_DYNAMIC;
	descConstBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	descConstBuffer.MiscFlags = 0u;
	descConstBuffer.ByteWidth = sizeof( consts );
	descConstBuffer.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA subresConstData = {};
	subresConstData.pSysMem = &consts;
	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateBuffer( &descConstBuffer, &subresConstData, &pConstantBuffer ) );
}

template<typename C>
void ConstantBuffer<C>::Update( Graphics& gfx, const C& consts )
{
	INFOMAN( gfx );

	D3D11_MAPPED_SUBRESOURCE subresMap;
	// subresMap will be filled with previous const buffer data
	// and subresMap gets access to the const buffer data
	GFX_CALL_THROW_INFO( GetContext( gfx )->Map(
		pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u,
		&subresMap
	) );
	// copy the data into subres pData memory
	memcpy( subresMap.pData, &consts, sizeof( consts ) );
	GetContext( gfx )->Unmap( pConstantBuffer.Get(), 0u );
}