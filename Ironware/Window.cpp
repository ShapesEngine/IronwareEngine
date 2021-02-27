/*!
 * \class WindowClass, Window, Exception, HrException
 *
 * \ingroup DEV
 *
 * \brief A class that controls windows and messages
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
#include "Window.h"
#include "resource.h"
#include "WindowExceptionMacros.h"
#include "imgui/imgui_impl_win32.h"

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
	width( width_in ),
	height( height_in )
{
	// =======================================================================
	// calculate window size based on desired client region size
	// -----------------------------------------------------------------------
	// TODO: assign 0 to the left and top sides of the rectangle
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	const DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	if( AdjustWindowRect( &wr, dwStyle, FALSE ) == 0 )
	{
		throw IRWND_LAST_EXCEPT();
	}
	// create window & get hWnd
	hWnd = CreateWindow(
		WindowClass::GetName(), name, dwStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this // we will retrieve [this] lpParam in HandleMsgSetup 
	);

	// check for the error
	if( hWnd == nullptr )
	{
		throw IRWND_LAST_EXCEPT();
	}

	// newly created windows start off as hidden, so we have to show it
	ShowWindow( hWnd, SW_SHOWDEFAULT );

	// Init ImGui Win32 Impl
	ImGui_ImplWin32_Init( hWnd );

	// create graphics object
	pGfx = std::make_unique<Graphics>( hWnd );
}

Window::~Window()
{
	ResetWindowProc();
	ImGui_ImplWin32_Shutdown();
	DestroyWindow( hWnd );
}

void Window::SetTitle( const std::wstring& title )
{
	if( SetWindowText( hWnd, title.c_str() ) == 0 )
	{
		throw IRWND_LAST_EXCEPT();
	}
}

void Window::ShowMessageBox( HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType )
{
	ResetWindowProc();
	MessageBox( hWnd, lpText, lpCaption, uType );
}

std::optional<int> Window::ProcessMessages() noexcept
{
	MSG msg;
	// while queue has messages, remove and dispatch them (but do not block on empty queue)
	while( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
	{
		// check for quit because peekmessage does not signal this via return val
		if( msg.message == WM_QUIT )
		{
			// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
			return (int)msg.wParam;
		}

		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	// return empty optional when not quitting app
	return {};
}

Graphics& Window::Gfx() const
{
	if( !pGfx )
	{
		throw IRWND_NOGFX_EXCEPT();
	}
	return *pGfx;
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
	if( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
	{
		return true;
	}

	switch( bool imGuiKbdCapture = ImGui::GetIO().WantCaptureKeyboard; msg )
	{
	case WM_CLOSE:
		PostQuitMessage( 0 );
		// we don't want the DefProc to handle this message because
		// our destructor will also try to destroy the window and it will lead to some problems,
		// so return 0 instead of break
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
		// stifle other keyboard messages if imgui wants to get full keyboard control
		if( imGuiKbdCapture )
		{
			break;
		}
		// filter autorepeat		
		if( !( lParam & 0x40000000 ) || kbd.AutorepeatIsEnabled() ) // 30 bits holds the previous key state
		{
			kbd.OnKeyPressed( static_cast<uint8_t>( wParam ) );
		}
		break;
	case WM_KEYUP:
		// Basic KEYUP messages applies also to system keys
	case WM_SYSKEYUP:
		// stifle other keyboard messages if imgui wants to get full keyboard control
		if( imGuiKbdCapture )
		{
			break;
		}
		kbd.OnKeyReleased( static_cast<uint8_t>( wParam ) );
		break;
	case WM_CHAR:
		// stifle other keyboard messages if imgui wants to get full keyboard control
		if( imGuiKbdCapture )
		{
			break;
		}
		kbd.OnChar( static_cast<wchar_t>( wParam ) );
		break;
		// =======================================================================

		// =======================================================================
		// Mouse Messages Handling
		// -----------------------------------------------------------------------
	case WM_MOUSEMOVE:
	{
		// stifle other keyboard messages if imgui wants to get full keyboard control
		if( imGuiKbdCapture )
		{
			break;
		}
		const POINTS pt = MAKEPOINTS( lParam );
		// in client region -> log move, and log enter + capture mouse (if not previously in window)
		if( pt.x >= 0 && pt.x < (SHORT)width && pt.y >= 0 && pt.y < (SHORT)height )
		{
			mouse.OnMouseMove( pt.x, pt.y );
			if( !mouse.IsInWindow() )
			{
				SetCapture( hWnd );
				mouse.OnMouseEnter();
			}
		}
		// not in client -> log move / maintain capture if button down
		else
		{
			if( mouse.AnyButtonIsPressed() )
			{
				mouse.OnMouseMove( pt.x, pt.y );
			}
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		// stifle other keyboard messages if imgui wants to get full keyboard control
		if( imGuiKbdCapture )
		{
			break;
		}
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnLeftPressed( pt.x, pt.y );
		break;
	}
	case WM_RBUTTONDOWN:
	{
		// stifle other keyboard messages if imgui wants to get full keyboard control
		if( imGuiKbdCapture )
		{
			break;
		}
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnRightPressed( pt.x, pt.y );
		break;
	}
	case WM_LBUTTONUP:
	{
		// stifle other keyboard messages if imgui wants to get full keyboard control
		if( imGuiKbdCapture )
		{
			break;
		}
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnLeftReleased( pt.x, pt.y );
		break;
	}
	case WM_MBUTTONDOWN:
	{
		// stifle other keyboard messages if imgui wants to get full keyboard control
		if( imGuiKbdCapture )
		{
			break;
		}
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnMiddlePressed( pt.x, pt.y );
		break;
	}	
	case WM_RBUTTONUP:
	{
		// stifle other keyboard messages if imgui wants to get full keyboard control
		if( imGuiKbdCapture )
		{
			break;
		}
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnRightReleased( pt.x, pt.y );
		break;
	}
	case WM_MBUTTONUP:
	{
		// stifle other keyboard messages if imgui wants to get full keyboard control
		if( imGuiKbdCapture )
		{
			break;
		}
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnMiddleReleased( pt.x, pt.y );
		break;
	}
	case WM_MOUSEWHEEL:
	{
		// stifle other keyboard messages if imgui wants to get full keyboard control
		if( imGuiKbdCapture )
		{
			break;
		}
		const POINTS pt = MAKEPOINTS( lParam );
		const int delta = GET_WHEEL_DELTA_WPARAM( wParam );
		mouse.OnWheelDelta( pt.x, pt.y, delta );
		break;
	}
	}
	// =======================================================================

	return DefWindowProc( hWnd, msg, wParam, lParam );
}
/******************************* WINDOW END ******************************/

/******************************* WINDOW EXCEPTION START ******************************/
Window::HrException::HrException( int line, const wchar_t* file, HRESULT hr ) noexcept :
	Exception( line, file ),
	hr( hr )
{}

const char* Window::HrException::what() const noexcept
{
	std::wostringstream woss;
	woss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (uint32_t)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = woss.str();
	return reinterpret_cast<const char*>( whatBuffer.c_str() );
}

std::wstring Window::Exception::TranslateErrorCode( HRESULT hr ) noexcept
{
	wchar_t* pMsgBuf = nullptr;

	// returns description string for the [hr] error code.
	// windows will allocate memory for err string and make our pointer point to it.
	const DWORD nMsgLen = FormatMessage(
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