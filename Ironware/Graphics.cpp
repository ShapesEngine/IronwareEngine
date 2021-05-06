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
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
#include "Graphics.h"
#include "IronUtils.h"
#include "GraphicsExceptionMacros.h"

#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include <sstream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

#pragma region Graphics

Graphics::Graphics( HWND hWnd )
{
	RECT windowRect;
	GetClientRect( hWnd, &windowRect );
	width = windowRect.right - windowRect.left;
	height = windowRect.bottom - windowRect.top;

	DXGI_SWAP_CHAIN_DESC descSwapChain = {};
	// D3D will automatically deduce width/height if it is 0
	descSwapChain.BufferDesc.Width = width;
	descSwapChain.BufferDesc.Height = height;
	descSwapChain.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	descSwapChain.BufferDesc.RefreshRate.Numerator = 0;
	descSwapChain.BufferDesc.RefreshRate.Denominator = 0;
	// setting as unspecified as we didn't set width and height
	descSwapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	descSwapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	// disable anti aliasing
	descSwapChain.SampleDesc.Count = 1;
	descSwapChain.SampleDesc.Quality = 0;
	descSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// 1 back & 1 front buffers => double buffering
	descSwapChain.BufferCount = 1;
	descSwapChain.OutputWindow = hWnd;
	descSwapChain.Windowed = TRUE;
	// TODO: consider using DXGI Flip Model
	descSwapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	descSwapChain.Flags = 0;
	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// for checking results of d3d functions
	HRESULT hr;

	// create device and front/back buffers, and swap chain and rendering context
	GFX_CALL_THROW_INFO( D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&descSwapChain,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pImmediateContext
	) );

	// gain access to texture subresource in swap chain (back buffer)
	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	// 0 is back buffer's index
	GFX_CALL_THROW_INFO( pSwapChain->GetBuffer( 0, __uuidof( ID3D11Resource ), &pBackBuffer ) );
	GFX_CALL_THROW_INFO( pDevice->CreateRenderTargetView( pBackBuffer.Get(), nullptr, &pRenderTargetView ) );

	wrl::ComPtr<ID3D11Texture2D> pDepth;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 0u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0u;
	descDepth.MiscFlags = 0u;
	GFX_CALL_THROW_INFO( pDevice->CreateTexture2D( &descDepth, nullptr, &pDepth ) );

	// create view of depth stencil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_CALL_THROW_INFO( pDevice->CreateDepthStencilView(
		pDepth.Get(), &descDSV, &pDepthStencilView
	) );

	// bind depth stencil view to OM
	pImmediateContext->OMSetRenderTargets( 1u, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get() );

	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0.f;
	vp.TopLeftY = 0.f;
	pImmediateContext->RSSetViewports( 1u, &vp );

	ImGui_ImplDX11_Init( pDevice.Get(), pImmediateContext.Get() );
}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
}

void Graphics::BeginFrame( float red, float green, float blue ) noexcept
{
	const float color[] = { red, green, blue, 1.f };
	pImmediateContext->ClearRenderTargetView( pRenderTargetView.Get(), color );
	// max depth 1.f
	pImmediateContext->ClearDepthStencilView( pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0u );

	// =======================================================================
	// imgui begin frame
	// -----------------------------------------------------------------------
	if( imGuiEnabled )
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
}

void Graphics::EndFrame()
{
	// =======================================================================
	// imgui frame end
	// -----------------------------------------------------------------------
	if( imGuiEnabled )
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
	}

	HRESULT hr;
#ifndef NDEBUG
	GFX_THROW_UNHANDLED_EXCEPTION
		infoManager.Set();
#endif
	// expected to consistently present at the present sync interval
	if( FAILED( hr = pSwapChain->Present( 1u, 0u ) ) )
	{
		if( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			throw GFX_DEVICE_REMOVED_EXCEPT( pDevice->GetDeviceRemovedReason() );
		}
		
		throw GFX_EXCEPT( hr );
	}
}

void Graphics::DrawIndexed( UINT count ) IFNOEXCEPT
{
	GFX_CALL_THROW_INFO_ONLY( pImmediateContext->DrawIndexed( count, 0u, 0u ) );
}

#pragma endregion Graphics

#pragma region GraphicsException

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

#pragma endregion GraphicsException