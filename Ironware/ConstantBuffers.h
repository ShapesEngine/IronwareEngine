/*!
 * \file ConstantBuffers.h
 * \date 2020/11/23 4:55
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A wrapper Constant Buffer classes and its child classes will be able to create cbuffer type buffers.
 * They will have Update function that makes it possible to update the existing buffer contents.
 *
 *
 * TODO:
 *
 * \note Each class in this file contains appropriate constant buffers
*/
#pragma once

#include "Bindable.h"
#include "GraphicsExceptionMacros.h"

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

	void Update( Graphics& gfx, const C& consts );	

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	UINT slot;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;
	__forceinline void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->VSSetConstantBuffers( slot, 1u, pConstantBuffer.GetAddressOf() ); }
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetConstantBuffers( slot, 1u, pConstantBuffer.GetAddressOf() ); }
};

#include "ConstantBuffers.tpp"