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

#include <d3d11.h>

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
		HrException( int line, const wchar_t* file, HRESULT hr ) noexcept;

		/**
		 * 	overridden function that will return type, error code, description and formatted string.
		 *
		 * \return const char_t* buffer
		 */
		const char* what() const noexcept override;	

		inline const wchar_t* GetType() const noexcept override { return L"Iron Graphics Exception"; }
		inline HRESULT GetErrorCode() const noexcept { return hr; }
		inline std::wstring GetErrorString() const noexcept { return DXGetErrorString( hr ); }
		std::wstring GetErrorDescription() const noexcept;

	private:
		HRESULT hr;
	};

	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;

	public:
		inline const wchar_t* GetType() const noexcept override { return L"Iron Graphics [DeviceRemoved] Exception"; }
	};

public:
	Graphics( HWND hWnd );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	~Graphics();

	void EndFrame();
	void ClearBuffer( float red, float green, float blue ) noexcept;

private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;	
	ID3D11DeviceContext* pImmediateContext = nullptr;
	ID3D11RenderTargetView* pRenderTargetView = nullptr;
};

