/*!
 * \file ConstantBuffers.h
 * \date 2020/11/23 4:55
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief 
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "Bindable.h"
#include "GraphicsExceptionMacros.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer( Graphics& gfx );
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