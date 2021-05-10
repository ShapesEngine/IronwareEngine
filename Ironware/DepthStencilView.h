/*!
 * \file DepthStencilView.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "Bindable.h"
#include "BufferResource.h"
#include "RenderTarget.h"

class DepthStencilView : public Bindable, public BufferResource
{
	friend class RenderTarget;
public:
	void BindAsBuffer( Graphics& gfx, BufferResource* renderTarget ) IFNOEXCEPT override;

	void BindAsBuffer( Graphics& gfx ) IFNOEXCEPT override { GetContext( gfx )->OMSetRenderTargets( 0u, nullptr, pDepthStencilView.Get() ); }
	void BindAsBuffer( Graphics& gfx, RenderTarget* rt ) IFNOEXCEPT { rt->BindAsBuffer( gfx, this ); }
	void Clear( Graphics& gfx ) IFNOEXCEPT override { GetContext( gfx )->ClearDepthStencilView( pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0u ); }
	std::wstring GetUID() const noexcept override { return L"?"; }

protected:
	DepthStencilView( Graphics& gfx, UINT width, UINT height, bool canBindShaderInput );

protected:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
};

class ShaderInputDepthStencil : public DepthStencilView
{
public:
	ShaderInputDepthStencil( Graphics& gfx, UINT slot );
	ShaderInputDepthStencil( Graphics& gfx, UINT width, UINT height, UINT slot );
	void Bind( Graphics& gfx ) IFNOEXCEPT override { GetContext( gfx )->PSSetShaderResources( slot,1u,pShaderResourceView.GetAddressOf() ); }

private:
	UINT slot;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
};

class OutputOnlyDepthStencil : public DepthStencilView
{
public:
	OutputOnlyDepthStencil( Graphics& gfx );
	OutputOnlyDepthStencil( Graphics& gfx, UINT width, UINT height );
	void Bind( Graphics& gfx ) IFNOEXCEPT override;
};