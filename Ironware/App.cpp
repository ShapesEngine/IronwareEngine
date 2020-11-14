/*!
 * \class App
 *
 * \ingroup DEV
 *
 * \brief A basic application class that is responsible for application specifig code managing
 *
 * TODO:
 *
 * \note 
 *
 * \author Magic
 *
 * \version 1.0
 *
 * \date November 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
#include "App.h"

App::App()
	:
	wnd( 640, 480, L"Ironware Engine" )
{}

int App::Begin()
{
	MSG msg;
	BOOL gResult;
	while( ( gResult = GetMessage( &msg, nullptr, 0, 0 ) ) > 0 )
	{
		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage( &msg );
		DispatchMessage( &msg );

		Tick();
	}

	// check if GetMessage call itself borked
	if( gResult == -1 )
	{
		throw IRWND_LAST_EXCEPT();
	}

	// wParam here is the value passed to PostQuitMessage
	return msg.wParam;
}

void App::Tick()
{

}