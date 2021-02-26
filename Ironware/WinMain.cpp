/*!
 * \file WinMain.cpp
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 *
 * 
 */
#include "App.h"
#include "IronUtils.h"

int WINAPI wWinMain(
	_In_		HINSTANCE hInstance,
	_In_opt_	HINSTANCE hPrevInstance,
	_In_		LPWSTR    lpCmdLine,
	_In_		int       nCmdShow )
{
	try
	{
		return App{}.Begin();
	}
	catch( const std::exception& e )
	{
		// check if the exception is type of IronException
		const IronException* pIe = dynamic_cast<const IronException*>( &e );
		/** GET THE APPROPRIATE EXCEPTION INFO */
		const std::wstring theErrorMSG = ( pIe ? CON_WCHREINT_CAST( pIe->what() ) : ToWide( e.what() ) );
		const wchar_t* theErrorType = ( pIe ? pIe->GetType() : L"Standard Exception" );
		/*===================================*/
		Window::ShowMessageBox( nullptr, theErrorMSG.c_str(), theErrorType, MB_OK | MB_ICONERROR );
	}
	catch( ... )
	{
		Window::ShowMessageBox( nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONERROR );
	}
	return -1;
}