/*!
 * \file Graphics.h
 * \date 2020/11/15 20:57
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
#include "dxerr.h"
#include "DxgiInfoManager.h"

#include <vector>
#include <d3d11.h>
#include <wrl.h>

class Graphics
{
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
		 * 	overridden function that will return type, error code, description and formatted string.
		 *
		 * \return const char_t* buffer
		 */
		const char* what() const noexcept override;	

		inline const wchar_t* GetType() const noexcept override { return L"Iron Graphics Exception"; }
		inline HRESULT GetErrorCode() const noexcept { return hr; }
		inline std::wstring GetErrorString() const noexcept { return DXGetErrorString( hr ); }
		inline std::wstring GetErrorInfo() const noexcept { return info; }
		std::wstring GetErrorDescription() const noexcept;

	private:
		HRESULT hr;
		std::wstring info;
	};

	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;

	public:
		inline const wchar_t* GetType() const noexcept override { return L"Iron Graphics [DeviceRemoved] Exception"; }
	};

	class InfoException : public Exception
	{
	public:
		InfoException( int line, const wchar_t* file, std::vector<std::wstring> infoMsgs = {} ) noexcept;

		const char* what() const noexcept override;
		inline const wchar_t* GetType() const noexcept override { return L"Iron Graphics [Info] Exception"; }
		inline std::wstring GetErrorInfo() const noexcept { return info; }
	private:
		std::wstring info;
	};

public:
	Graphics( HWND hWnd );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	~Graphics() = default;

	void EndFrame();
	void ClearBuffer( float red, float green, float blue ) noexcept;
	void DrawTriangle( float angle, float x, float y );

private:
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
};

