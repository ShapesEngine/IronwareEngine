/*!
 * \file ConstantBufferEx.h
 * \date 2021/04/29 19:04
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief
 *
 * TODO: Create extended version of vertex constant buffer
 *
 * \note A header file that contains extended version of ConstantBuffers
*/
#pragma once

#include "Bindable.h"
#include "GraphicsExceptionMacros.h"
#include "DynamicConstantBuffer.h"
#include "TechniqueProbe.h"

class ConstantBufferEx : public Bindable
{
public:
	void Update( Graphics& gfx, const Buffer& buf );
	virtual const LayoutElement& GetRootLayoutElement() const noexcept = 0;
	std::wstring GetUID() const noexcept override;

protected:
	ConstantBufferEx( Graphics& gfx, const LayoutElement& layoutRoot, UINT slot, const Buffer* pBuf );

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	UINT slot;
};

class PixelConstantBufferEx : public ConstantBufferEx
{
public:
	using ConstantBufferEx::ConstantBufferEx;
	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetConstantBuffers( slot, 1u, pConstantBuffer.GetAddressOf() ); }
};

class VertexConstantBufferEx : public ConstantBufferEx
{
public:
	using ConstantBufferEx::ConstantBufferEx;
	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->VSSetConstantBuffers( slot, 1u, pConstantBuffer.GetAddressOf() ); }
};

template<class T>
class CachingConstantBufferEx : public T
{
public:
	CachingConstantBufferEx( Graphics& gfx, const CookedLayout& layout, UINT slot ) :
		T( gfx, *layout.ShareRoot(), slot, nullptr ),
		buf( Buffer( layout ) )
	{}
	CachingConstantBufferEx( Graphics& gfx, const Buffer& buf, UINT slot ) :
		T( gfx, buf.GetRootLayoutElement(), slot, &buf ),
		buf( buf )
	{}
	void Bind( Graphics& gfx ) noexcept override
	{
		if( dirty )
		{
			T::Update( gfx, buf );
			dirty = false;
		}
		T::Bind( gfx );
	}

	void SetBuffer( const Buffer& buf_in )
	{
		buf.CopyFrom( buf_in );
		dirty = true;
	}

	void Accept( TechniqueProbe& probe ) override
	{
		if( probe.VisitBuffer( buf ) )
		{
			dirty = true;
		}
	}

	const LayoutElement& GetRootLayoutElement() const noexcept override { return buf.GetRootLayoutElement(); }
	const Buffer& GetBuffer() const noexcept { return buf; }

private:
	bool dirty = false;
	Buffer buf;
};

using CachingPixelConstantBufferEx = CachingConstantBufferEx<PixelConstantBufferEx>;
using CachingVertexConstantBufferEx = CachingConstantBufferEx<VertexConstantBufferEx>;