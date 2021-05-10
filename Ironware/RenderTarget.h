#pragma once
#include "Bindable.h"
#include "BufferResource.h"

class Graphics;

class DepthStencilView;

class RenderTarget : public Bindable, public BufferResource
{
public:
	void BindAsBuffer( Graphics& gfx ) IFNOEXCEPT override;
	void BindAsBuffer( Graphics& gfx, BufferResource* depthStencil ) IFNOEXCEPT override;
	void BindAsBuffer( Graphics& gfx, DepthStencilView* depthStencil ) IFNOEXCEPT;
	void Clear( Graphics& gfx ) IFNOEXCEPT override;
	void Clear( Graphics& gfx, const std::array<float, 4>& color ) IFNOEXCEPT;
	UINT GetWidth() const noexcept;
	UINT GetHeight() const noexcept;
	std::wstring GetUID() const noexcept override { return L"?"; }

private:
	void BindAsBuffer( Graphics& gfx, ID3D11DepthStencilView* pDepthStencilView ) IFNOEXCEPT;

protected:
	RenderTarget( Graphics& gfx, ID3D11Texture2D* pTexture );
	RenderTarget( Graphics& gfx, UINT width, UINT height );
	UINT width;
	UINT height;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
};

class ShaderInputRenderTarget : public RenderTarget
{
public:
	ShaderInputRenderTarget( Graphics& gfx, UINT width, UINT height, UINT slot );
	void Bind( Graphics& gfx ) IFNOEXCEPT override;

private:
	UINT slot;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
};

// RT for Graphics to create RenderTarget for the back buffer
class OutputOnlyRenderTarget : public RenderTarget
{
	friend Graphics;
public:
	void Bind( Graphics& gfx ) IFNOEXCEPT override;

private:
	OutputOnlyRenderTarget( Graphics& gfx, ID3D11Texture2D* pTexture );
};