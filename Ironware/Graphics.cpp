/*!
 * \class Graphics, HrException(Exception)
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
#include "IronUtils.h"

#include <sstream>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

 // =======================================================================
 // graphics exception checking/throwing macros (some with dxgi infos)
 // -----------------------------------------------------------------------
#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException( __LINE__, WFILE, (hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__, WFILE, hr )

#ifndef NDEBUG
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_GET_EXCEPT_ERROR_TYPE(hr)
#define GFX_GET_EXCEPT_ERROR_TYPE(hr) (hr == DXGI_ERROR_DEVICE_REMOVED ? GFX_DEVICE_REMOVED_EXCEPT(hr) : GFX_EXCEPT(hr))
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__, WFILE, (hr), infoManager.GetMessages() )
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__, WFILE, (hr), infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); GFX_THROW_UNHANDLED_EXCEPTION
#define GFX_THROW_UNHANDLED_EXCEPTION {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,WFILE,v);}}
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__, WFILE, (hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__, WFILE, (hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif
// =======================================================================

namespace wrl = Microsoft::WRL;

/******************************* GRAPHICS START ******************************/
Graphics::Graphics( HWND hWnd )
{
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferDesc.Width = 0;
	scd.BufferDesc.Height = 0;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	// don't set if you haven't set width and height, as they should be equal to the 
	// output width and height of the window
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
	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// for checking results of d3d functions
	HRESULT hr;

	// create device and front/back buffers, and swap chain and rendering context
	GFX_THROW_INFO( D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
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
	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	// 0 is back buffer's index
	GFX_THROW_INFO( pSwapChain->GetBuffer( 0, __uuidof( ID3D11Resource ), &pBackBuffer ) );
	GFX_THROW_INFO( pDevice->CreateRenderTargetView( pBackBuffer.Get(), nullptr, &pRenderTargetView ) );
}

void Graphics::EndFrame()
{
	HRESULT hr;
#ifndef NDEBUG
	GFX_THROW_UNHANDLED_EXCEPTION
	infoManager.Set();
#endif
	// sync interval = 60; expected to consistently present at this rate
	if( FAILED( hr = pSwapChain->Present( 1u, 0u ) ) )
	{
		if( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			throw GFX_DEVICE_REMOVED_EXCEPT( pDevice->GetDeviceRemovedReason() );
		}
		else
		{
			throw GFX_EXCEPT( hr );
		}
	}
}

void Graphics::ClearBuffer( float red, float green, float blue ) noexcept
{
	const float color[] = { red, green, blue, 1.f };
	pImmediateContext->ClearRenderTargetView( pRenderTargetView.Get(), color );
}

void Graphics::DrawTriangle()
{
	HRESULT hr;

	struct Vertex
	{
		float x;
		float y;
	};

	const Vertex vertices[] = {
		{ 0.f, 0.5f },
		{ 0.5f, -0.5f },
		{ -0.5f, -0.5f }
	};

	D3D11_BUFFER_DESC bufferDesc = {};	
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = sizeof( vertices );
	bufferDesc.StructureByteStride = sizeof( Vertex );
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;	

	D3D11_SUBRESOURCE_DATA subresData = {};
	subresData.pSysMem = vertices;

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	GFX_THROW_INFO( pDevice->CreateBuffer( &bufferDesc, &subresData, &pVertexBuffer ) );

	const UINT stride = (UINT)sizeof( Vertex );
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset );

	// =======================================================================
	// create vertex shader
	// -----------------------------------------------------------------------
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO( D3DReadFileToBlob( L"VertexShader.cso", &pBlob ) );
	GFX_THROW_INFO( pDevice->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader ) );

	// bind vertex shader
	pImmediateContext->VSSetShader( pVertexShader.Get(), nullptr, 0u );

	// =======================================================================
	// create pixel shader
	// -----------------------------------------------------------------------
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	GFX_THROW_INFO( D3DReadFileToBlob( L"PixelShader.cso", &pBlob ) );
	GFX_THROW_INFO( pDevice->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader ) );

	// bind pixel shader
	pImmediateContext->PSSetShader( pPixelShader.Get(), nullptr, 0u );
	
	pImmediateContext->OMSetRenderTargets( 1u, pRenderTargetView.GetAddressOf(), nullptr );

	
	pImmediateContext->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST ); // Triangle list is group of 3 vertices

	// =======================================================================
	// configure viewport
	// -----------------------------------------------------------------------
	D3D11_VIEWPORT viewport;
	viewport.Width = 640.f;
	viewport.Height = 480.f;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;

	pImmediateContext->RSSetViewports( 1u, &viewport );

	GFX_THROW_INFO_ONLY( pImmediateContext->Draw( (UINT)std::size(vertices), 0u ) );
}
/******************************* GRAPHICS END ******************************/

/******************************* GRAPHICS EXCEPTION START ******************************/
Graphics::HrException::HrException( int line, const wchar_t* file, HRESULT hr, std::vector<std::wstring> infoMsgs ) noexcept :
	Exception( line, file ),
	hr( hr )
{
	// join all info messages with newlines into single string
	for( const auto& m : infoMsgs )
	{
		info += m;
		info.push_back( '\n' );
	}
	// remove final newline if exists
	if( !info.empty() )
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::wostringstream woss;
	woss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if( !info.empty() )
	{
		woss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	woss << GetOriginString();
	whatBuffer = woss.str();
	return CON_CHREINT_CAST( whatBuffer.c_str() );
}

std::wstring Graphics::HrException::GetErrorDescription() const noexcept
{
	wchar_t buf[512];
	// use _countof, because this may cause buffer overrun
	// the writeable size is 1024 bytes
	// but 2048 bytes might be written
	DXGetErrorDescription( hr, buf, _countof( buf ) );
	return buf;
}

Graphics::InfoException::InfoException( int line, const wchar_t* file, std::vector<std::wstring> infoMsgs ) noexcept :
	Exception( line, file )
{
	// join all info messages with newlines into single string
	for( const auto& m : infoMsgs )
	{
		info += m;
		info.push_back( '\n' );
	}
	// remove final newline if exists
	if( !info.empty() )
	{
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept
{
	std::wostringstream woss;
	woss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	woss << GetOriginString();
	whatBuffer = woss.str();

	return CON_CHREINT_CAST( whatBuffer.c_str() );
}
/******************************* GRAPHICS EXCEPTION END ******************************/