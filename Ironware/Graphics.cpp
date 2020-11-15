/*!
 * \class Graphics, HrException
 *
 * \ingroup DEV
 *
 * \brief A class that's responsible for the graphical output
 *
 * TODO:
 *
 * \note 
 *
 * \author Yernar Aldabergenov
 *
 * \version 1.0
 *
 * \date November 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
#include "Graphics.h"

#include <sstream>

#pragma comment(lib, "d3d11.lib")

#define GFX_THROW_FAILED(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__, WFILE, hr )
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__, WFILE, (hr) )

Graphics::Graphics( HWND hWnd )
{
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferDesc.Width = 0;
	scd.BufferDesc.Height = 0;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	// dont set if you havent set width and height
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	// disable anti aliasing
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// back buffer count
	scd.BufferCount = 1;
	scd.OutputWindow = hWnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0;

	// for checking results of d3d functions
	HRESULT hr;

	// create device and front/back buffers, and swap chain and rendering context
	GFX_THROW_FAILED( D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pImmediateContext 
	) );

	// gain access to texture subresource in swap chain (back buffer)
	ID3D11Resource* pBackBuffer = nullptr;
	GFX_THROW_FAILED( pSwapChain->GetBuffer( 0, __uuidof( ID3D11Resource ), reinterpret_cast<void**>( &pBackBuffer ) ) );
	GFX_THROW_FAILED( pDevice->CreateRenderTargetView( pBackBuffer, nullptr, &pRenderTargetView ) );
	pBackBuffer->Release();
}

Graphics::~Graphics()
{
	if( pRenderTargetView )
	{
		pRenderTargetView->Release();
	}
	if( pImmediateContext )
	{
		pImmediateContext->Release();
	}
	if( pSwapChain )
	{
		pSwapChain->Release();
	}
	if( pDevice )
	{
		pDevice->Release();
	}
}

void Graphics::EndFrame()
{
	HRESULT hr;
	// sync interval = 60; expected to consistently present at this rate
	if( FAILED( hr = pSwapChain->Present( 1u, 0u ) ) )
	{
		if( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			throw GFX_DEVICE_REMOVED_EXCEPT( pDevice->GetDeviceRemovedReason() );
		}
		else
		{
			GFX_THROW_FAILED( hr );
		}
	}
}

void Graphics::ClearBuffer( float red, float green, float blue ) noexcept
{
	const float color[] = { red, green, blue, 1.f };
	pImmediateContext->ClearRenderTargetView( pRenderTargetView, color );
}

/******************************* Graphics Exception Start ******************************/
Graphics::HrException::HrException( int line, const wchar_t* file, HRESULT hr ) noexcept :
	Exception( line, file ),
	hr( hr )
{}

const char* Graphics::HrException::what() const noexcept
{
	std::wostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return reinterpret_cast<const char*>( whatBuffer.c_str() );
}

std::wstring Graphics::HrException::GetErrorDescription() const noexcept
{
	wchar_t buf[512];
	DXGetErrorDescription( hr, buf, sizeof( buf ) );
	return buf;
}
/******************************* Graphics Exception End ******************************/
