#include "Window.h"

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
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIcon = nullptr;
	RegisterClassEx( &wc );
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass( wndClassName, GetInstance() );
}

Window::Window( int width, int height, const char* name ) noexcept
{
	// =======================================================================
	// calculate window size based on desired client region size
	// -----------------------------------------------------------------------
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect( &wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE );

	// create window & get hWnd
	hWnd = CreateWindow( 
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	ShowWindow( hWnd, SW_SHOWDEFAULT );
}

Window::~Window()
{
	DestroyWindow( hWnd );
}

LRESULT Window::HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
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

LRESULT Window::HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
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

		// we will call destructor by ourselves preventing double deletion of the window object
		return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}
