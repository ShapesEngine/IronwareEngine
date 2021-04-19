/*!
 * \file ConstantBuffers.h
 * \date 2020/09/23 4:55
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A header that contains wrapper (bindable) Constant Buffer classes and its child classes.
 * * That will be able to create cbuffer type buffers.
 * * They will have Update function that makes it possible to update the existing buffer contents.
 *
 * \note Each class in this file contains appropriate constant buffers
*/
#pragma once

#include "Bindable.h"
#include "BindableCollection.h"
#include "GraphicsExceptionMacros.h"
#include "IronUtils.h"

/*!
 * \class ConstantBuffer
 *
 * \ingroup Bindables
 *
 * \brief Base CBuffer class that contains various functions for binding & setting buffers
 *
 * TODO:
 *
 * \note
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
template<typename C>
class ConstantBuffer : public Bindable
{
public:
	/**
	 * @brief Create constant buffer without initializing the buffer
	 * @param gfx Graphics object needed to get access to the pDevice
	 * @param slot binds resource to the specified slot
	*/
	ConstantBuffer( Graphics& gfx, UINT slot = 0u );

	/**
	 * @brief Create constant buffer and initialize the buffer with data
	 * @param gfx Graphics object needed to get access to the pDevice
	 * @param consts buffer const data
	 * @param slot binds resource to the specified slot
	*/
	ConstantBuffer( Graphics& gfx, const C& consts, UINT slot = 0u );

	/**
	 * @brief Updates the current constant buffer with a new one
	 * @param gfx Graphics object needed to get access to the pContext
	 * @param consts Source constant buffer
	*/
	void Update( Graphics& gfx, const C& consts );

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	UINT slot;
};

/*!
 * \class VertexConstantBuffer
 *
 * \ingroup Bindables
 *
 * \brief Responsible class for binding & setting vertex shader constant buffers
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->VSSetConstantBuffers( slot, 1u, pConstantBuffer.GetAddressOf() ); }
	static std::shared_ptr<VertexConstantBuffer> Resolve( Graphics& gfx, const C& consts, UINT slot = 0u ) { return BindableCollection::Resolve<VertexConstantBuffer>( gfx, consts, slot ); }
	static std::shared_ptr<VertexConstantBuffer> Resolve( Graphics& gfx, UINT slot = 0u ) { return BindableCollection::Resolve<VertexConstantBuffer>( gfx, slot ); }
	static std::wstring GenerateUID( const C& consts, UINT slot ) { return GenerateUID( slot ); }
	static std::wstring GenerateUID( UINT slot = 0u ) { return GET_CLASS_WNAME( VertexConstantBuffer ) + L"#" + std::to_wstring( slot ); }
	std::wstring GetUID() const noexcept override { return GenerateUID( slot ); }
};

/*!
 * \class PixelConstantBuffer
 *
 * \ingroup Bindables
 *
 * \brief Responsible class for binding & setting pixel shader constant buffers
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetConstantBuffers( slot, 1u, pConstantBuffer.GetAddressOf() ); }
	static std::shared_ptr<PixelConstantBuffer> Resolve( Graphics& gfx, const C& consts, UINT slot = 0u ) { return BindableCollection::Resolve<PixelConstantBuffer<C>>( gfx, consts, slot ); }
	static std::shared_ptr<PixelConstantBuffer> Resolve( Graphics& gfx, UINT slot = 0u ) { return BindableCollection::Resolve<PixelConstantBuffer<C>>( gfx, slot ); }
	static std::wstring GenerateUID( const C& consts, UINT slot ) { return GenerateUID( slot ); }
	static std::wstring GenerateUID( UINT slot = 0u ) { return GET_CLASS_WNAME( PixelConstantBuffer ) + L"#" + std::to_wstring( slot ); }
	std::wstring GetUID() const noexcept override { return GenerateUID( slot ); }
};

#pragma region implementation

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

#pragma endregion implementation