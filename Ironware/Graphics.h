/*!
 * \file Graphics.h
 * \date 2020/09/15 20:57
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A class that's responsible for the graphical output
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "IronWin.h"
#include "IronException.h"
#include "CommonMacros.h"
#include <DXErr/dxerr.h>
#include "DxgiInfoManager.h"

#include <vector>
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <memory>
#include <random>

class Graphics
{
	friend class GraphicsResource;
public:
	class Exception : public IronException
	{
		using IronException::IronException;
	};

	class HrException : public Exception
	{
	public:
		HrException( int line, const wchar_t* file, HRESULT hr, std::vector<std::wstring> infoMsgs = {} ) noexcept;

		/**
		 * @brief overridden function that will return type, error code, description and formatted string
		 * @return const char_t* buffer
		*/
		const char* what() const noexcept override;

		const wchar_t* GetType() const noexcept override { return L"Iron Graphics Exception"; }
		HRESULT GetErrorCode() const noexcept { return hr; }
		std::wstring GetErrorString() const noexcept { return DXGetErrorString( hr ); }
		std::wstring GetErrorInfo() const noexcept { return info; }
		std::wstring GetErrorDescription() const noexcept;

	private:
		HRESULT hr;
		std::wstring info;
	};

	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;

	public:
		const wchar_t* GetType() const noexcept override { return L"Iron Graphics [DeviceRemoved] Exception"; }
	};

	class InfoException : public Exception
	{
	public:
		InfoException( int line, const wchar_t* file, std::vector<std::wstring> infoMsgs = {} ) noexcept;

		const char* what() const noexcept override;
		const wchar_t* GetType() const noexcept override { return L"Iron Graphics [Info] Exception"; }
		std::wstring GetErrorInfo() const noexcept { return info; }
	private:
		std::wstring info;
	};

public:
	Graphics( HWND hWnd );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	~Graphics();

	void BeginFrame( float red, float green, float blue ) noexcept;
	void EndFrame();
	void DrawIndexed( UINT count ) IFNOEXCEPT;
	void BindSwapBuffer() noexcept;
	void BindSwapBuffer( const class DepthStencilView& dsv ) noexcept;

	UINT GetWidth() const noexcept { return width; }
	UINT GetHeight() const noexcept { return height; }
	void SetCamera( DirectX::FXMMATRIX cam ) noexcept { camera = cam; }
	DirectX::XMMATRIX GetCamera() const noexcept { return camera; }
	void SetProjection( DirectX::FXMMATRIX proj ) noexcept { projection = proj; }
	DirectX::XMMATRIX GetProjection() const noexcept { return projection; }
	void EnableImGui() noexcept { imGuiEnabled = true; }
	void DisableImGui() noexcept { imGuiEnabled = false; }
	bool IsImGuiEnabled() const noexcept { return imGuiEnabled; }

private:
	DirectX::XMMATRIX projection = {};
	DirectX::XMMATRIX camera = {};
	bool imGuiEnabled = true;
	UINT width = 0u;
	UINT height = 0u;

#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
};