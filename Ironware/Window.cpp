#include "Window.h"
#include "resource.h"

#include <sstream>

/******************************* WINDOW CLASS START ******************************/
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept :
    hInst( GetModuleHandle( nullptr ) )
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof( wc );
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>( LoadImage( GetInstance(), MAKEINTRESOURCE( IDI_ICON1 ),
											  IMAGE_ICON, 32, 32, 0 ) );
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	// TODO: make small new icon with 16x16 dimensions
	wc.hIconSm = static_cast<HICON>( LoadImage( GetInstance(), MAKEINTRESOURCE( IDI_ICON1 ),
												IMAGE_ICON, 32, 32, 0 ) );
	RegisterClassEx( &wc );
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass( wndClassName, GetInstance() );
}
/******************************* WINDOW CLASS END ******************************/

/******************************* WINDOW START ******************************/
Window::Window( int width_in, int height_in, const wchar_t* name ) :
	width(width_in),
	height(height_in)
{
	// =======================================================================
	// calculate window size based on desired client region size
	// -----------------------------------------------------------------------
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if( FAILED( AdjustWindowRect( &wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE ) ) )
	{
		throw IRWND_LAST_EXCEPT();
	};

	// create window & get hWnd
	hWnd = CreateWindow( 
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	// check for error
	if( hWnd == nullptr )
	{
		throw IRWND_LAST_EXCEPT();
	}

	ShowWindow( hWnd, SW_SHOWDEFAULT );
}

Window::~Window()
{
	DestroyWindow( hWnd );
}

LRESULT CALLBACK Window::HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if( msg == WM_NCCREATE )
	{
		// =======================================================================
		// extract ptr to window class from creation data
		// -----------------------------------------------------------------------
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>( lParam );
		Window* const pWnd = static_cast<Window*>( pCreate->lpCreateParams );

		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pWnd ) );
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr( hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( &Window::HandleMsgThunk ) );
		
		// forward message to window class handler
		return pWnd->HandleMsg( hWnd, msg, wParam, lParam );
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK Window::HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	// retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

	// forward message to window class handler
	return pWnd->HandleMsg( hWnd, msg, wParam, lParam );
}

LRESULT Window::HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	switch( msg )
	{
	case WM_CLOSE:
		PostQuitMessage( 0 );
		// we don't want the DefProc to handle this message because
		// we want our destructor to destroy the window, so return 0 instead of break
		return 0;
	
	// clear keystate when window loses focus to prevent input getting "stuck"
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	// =======================================================================
	// Keyboard Messages Handling
	// -----------------------------------------------------------------------	
	case WM_KEYDOWN:
	// SYSKEY messages need to be handled to track system keys such as ALT, F10, etc.
	// Basic KEYDOWN messages applies also to system keys
	case WM_SYSKEYDOWN:
		// filter autorepeat		
		if( !( lParam & 0x40000000 ) || kbd.AutorepeatIsEnabled() ) // 30 bits hold the KEYDOWN message for repeating keys
		{
			kbd.OnKeyPressed( static_cast<unsigned char>( wParam ) );
		}
		break;
	case WM_KEYUP:
	// Basic KEYUP messages applies also to system keys
	case WM_SYSKEYUP:
		kbd.OnKeyReleased( static_cast<unsigned char>( wParam ) );
		break;
	case WM_CHAR:
		kbd.OnChar( static_cast<wchar_t>( wParam ) );
		break;
	}
	// =======================================================================

	return DefWindowProc( hWnd, msg, wParam, lParam );
}
/******************************* WINDOW END ******************************/

/******************************* WINDOW EXCEPTION START ******************************/
Window::Exception::Exception( int line, const wchar_t* file, HRESULT hr ) noexcept :
	IronException( line, file ),
	hr( hr )
{}

const char* Window::Exception::what() const noexcept
{
	std::wostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return reinterpret_cast<const char*>( whatBuffer.c_str() );
}

std::wstring Window::Exception::TranslateErrorCode( HRESULT hr ) noexcept
{
	wchar_t* pMsgBuf = nullptr;

	// returns description string for the [hr] error code.
	// windows will allocate memory for err string and make our pointer point to it.
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		reinterpret_cast<LPWSTR>( &pMsgBuf ), 0, nullptr
	);

	// 0 string length returned indicates a failure
	if( nMsgLen == 0 )
	{
		return L"Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::wstring errorString = pMsgBuf;
	// free windows buffer
	LocalFree( pMsgBuf );
	return errorString;
}
/******************************* WINDOW EXCEPTION END ******************************/