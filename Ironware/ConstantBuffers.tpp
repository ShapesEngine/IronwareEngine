/*!
 * \file ConstantBuffers.tpp
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 *
 * 
 */
template<typename C>
ConstantBuffer<C>::ConstantBuffer( Graphics& gfx )
{
	INFOMAN( gfx );

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof( C );
	cbd.StructureByteStride = 0u;
	GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &cbd, nullptr, &pConstantBuffer ) );
}

template<typename C>
ConstantBuffer<C>::ConstantBuffer( Graphics& gfx, const C& consts )
{
	INFOMAN( gfx );

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof( consts );
	cbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &consts;
	GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &cbd, &csd, &pConstantBuffer ) );
}

template<typename C>
void ConstantBuffer<C>::Update( Graphics& gfx, const C& consts )
{
	INFOMAN( gfx );

	D3D11_MAPPED_SUBRESOURCE msr;
	GFX_THROW_INFO( GetContext( gfx )->Map(
		pConstantBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u,
		&msr
	) );
	memcpy( msr.pData, &consts, sizeof( consts ) );
	GetContext( gfx )->Unmap( pConstantBuffer.Get(), 0u );
}