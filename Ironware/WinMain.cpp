#include "App.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow )
{
	try
	{
		return App{}.Begin();
	}
	catch( const IronException& e )
	{
		SetWindowLongPtr( GetActiveWindow(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( &DefWindowProc ) );
		MessageBox( nullptr, WCON_CHREINT_CAST( e.what() ), WCON_CHREINT_CAST( e.GetType() ), MB_OK | MB_ICONEXCLAMATION );
	}
	catch( const std::exception& e )
	{
		SetWindowLongPtr( GetActiveWindow(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( &DefWindowProc ) );
		MessageBox( nullptr, WCON_CHREINT_CAST( e.what() ), L"Standard Exception", MB_OK | MB_ICONEXCLAMATION );
	}
	catch( ... )
	{
		SetWindowLongPtr( GetActiveWindow(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( &DefWindowProc ) );
		MessageBox( nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION );
	}
	return -1;
}