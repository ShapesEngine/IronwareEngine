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
 * \note They contain appropriate constant buffers
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
	*/
	ConstantBuffer( Graphics& gfx );
	/**
	 * @brief Create constant buffer and initialize the buffer with data
	 * @param gfx Graphics object needed to get access to the pDevice 
	 * @param consts buffer const data
	*/
	ConstantBuffer( Graphics& gfx, const C& consts );

	void Update( Graphics& gfx, const C& consts );	

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;
	inline void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->VSSetConstantBuffers( 0u, 1u, pConstantBuffer.GetAddressOf() ); }
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetConstantBuffers( 0u, 1u, pConstantBuffer.GetAddressOf() ); }
};

#include "ConstantBuffers.tpp"