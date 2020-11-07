#include <Windows.h>
#include "WindowsMessageMap.h"
#include <string>
#include <sstream>

constexpr auto pWindowName = "Ironware Engine";

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static WindowsMessageMap wmm;
	OutputDebugString( wmm( msg, lParam, wParam ).c_str() );

	switch( msg )
	{
	case WM_CLOSE:
		PostQuitMessage( 1 );
		break;
	case WM_KEYDOWN:		
		SetWindowText( hWnd, std::to_string(wParam).c_str() );
		break;
	case WM_KEYUP:
		if( wParam == '\r' )
		{
			SetWindowText( hWnd, pWindowName );
		}		
		break;
	case WM_CHAR:
		{
			static std::string title;
			title.push_back( (char)wParam );
			if( wParam == 8 )
			{
				title.clear();
			}
			SetWindowText( hWnd, title.c_str() );
		}
		break;
	case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			static std::ostringstream oss;
			oss << "(" << pt.x << ", " << pt.y << ")";
			SetWindowText( hWnd, oss.str().c_str() );
		}
		break;
	default:
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

int CALLBACK WinMain( 
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nShowCmd 
)
{
	constexpr auto pClassName = "CNIronware";	

	//////////////////////////////////////////////////////////////////////////
	// Register Windows Class
	// -----------------------------------------------------------------------
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof( wc );
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIcon = nullptr;
	RegisterClassEx( &wc );

	//////////////////////////////////////////////////////////////////////////
	// Create Window Instance
	// -----------------------------------------------------------------------  
	HWND hWnd = CreateWindowEx(
		0, pClassName,
		pWindowName,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		0, 0, 640, 480,
		nullptr, nullptr,
		hInstance, nullptr
	);
	//////////////////////////////////////////////////////////////////////////
	// Show the Window
	// -----------------------------------------------------------------------
	ShowWindow( hWnd, SW_SHOW );

	//////////////////////////////////////////////////////////////////////////
	// Process Message
	// -----------------------------------------------------------------------
	MSG msg;
	BOOL res;
	while( ( res = GetMessage( &msg, nullptr, 0, 0 ) ) > 0 )
	{
		if( res == -1 )
		{
			return -1;
		}
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	
	return msg.wParam;
}