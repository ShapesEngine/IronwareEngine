#include "App.h"
#include "IronUtils.h"

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow )
{
	try
	{
		return App{}.Begin();
	}
	catch( const std::exception& e )
	{
		const IronException* pIe = dynamic_cast<const IronException*>( &e );
		const std::wstring theErrorMSG = ( pIe ? CON_WCHREINT_CAST( pIe->what() ) : ToWide( e.what() ) );
		const wchar_t* theErrorType = ( pIe ? pIe->GetType() : L"Standard Exception" );
		SAFE_MESSAGEBOX( nullptr, theErrorMSG.c_str(), theErrorType, MB_OK | MB_ICONERROR );
	}
	catch( ... )
	{
		SAFE_MESSAGEBOX( nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONERROR );
	}
	return -1;
}