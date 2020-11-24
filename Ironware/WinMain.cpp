#include "App.h"
#include "IronUtils.h"

int CALLBACK wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow )
{
	try
	{
		return App{}.Begin();
	}
	catch( const IronException& e )
	{
		SAFE_MESSAGEBOX( nullptr, CON_WCHREINT_CAST( e.what() ), e.GetType(), MB_OK | MB_ICONERROR );
	}
	catch( const std::exception& e )
	{
		SAFE_MESSAGEBOX( nullptr, ToWide( e.what() ).c_str(), L"Standard Exception", MB_OK | MB_ICONERROR );
	}
	catch( ... )
	{
		SAFE_MESSAGEBOX( nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONERROR );
	}
	return -1;
}