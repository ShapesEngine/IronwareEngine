#include "App.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow )
{
	try
	{
		return App{}.Go();
	}
	catch( const IronException& e )
	{
		MessageBox( nullptr, WCCHREINT_CAST( e.what() ), WCCHREINT_CAST( e.GetType() ), MB_OK | MB_ICONEXCLAMATION );
	}
	catch( const std::exception& e )
	{
		MessageBox( nullptr, WCCHREINT_CAST( e.what() ), L"Standard Exception", MB_OK | MB_ICONEXCLAMATION );
	}
	catch( ... )
	{
		MessageBox( nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION );
	}
	return -1;
}