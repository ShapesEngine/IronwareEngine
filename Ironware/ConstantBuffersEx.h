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

class PixelConstantBufferEx : public Bindable
{
public:
	void Update( Graphics& gfx, const Buffer& buf );
	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetConstantBuffers( slot, 1u, pConstantBuffer.GetAddressOf() ); }
	virtual const LayoutElement& GetRootLayoutElement() const noexcept = 0;

protected:
	PixelConstantBufferEx( Graphics& gfx, const LayoutElement& layoutRoot, UINT slot, const Buffer* pBuf );

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	UINT slot;
};

class CachingPixelConstantBufferEx : public PixelConstantBufferEx
{
public:
	CachingPixelConstantBufferEx( Graphics& gfx, const CookedLayout& layout, UINT slot );
	CachingPixelConstantBufferEx( Graphics& gfx, const Buffer& buf, UINT slot );
	const LayoutElement& GetRootLayoutElement() const noexcept override { return buf.GetRootLayoutElement(); }
	const Buffer& GetBuffer() const noexcept { return buf; }
	void SetBuffer( const Buffer& buf_in );
	void Bind( Graphics& gfx ) noexcept override;

private:
	bool dirty = false;
	Buffer buf;
};

class NocachePixelConstantBufferEx : public PixelConstantBufferEx
{
public:
	NocachePixelConstantBufferEx( Graphics& gfx, const CookedLayout& layout, UINT slot );
	NocachePixelConstantBufferEx( Graphics& gfx, const Buffer& buf, UINT slot );
	const LayoutElement& GetRootLayoutElement() const noexcept override { return *pLayoutRoot; }

private:
	std::shared_ptr<LayoutElement> pLayoutRoot;
};