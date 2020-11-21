/*!
 * \class App
 *
 * \ingroup DEV
 *
 * \brief A basic application class that is responsible for application specific code managing
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
#include "App.h"

App::App() :
	wnd( 640, 480, L"Ironware Engine" )
{}

int App::Begin()
{
	MSG msg;
	BOOL gResult;
	while( true )
	{
		// process all messages pending, but to not block for new messages
		if( const auto ecode = Window::ProcessMessages() )
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		Tick();
	}

	// check if GetMessage call itself worked
	if( gResult == -1 )
	{
		throw IRWND_LAST_EXCEPT();
	}

	// wParam here is the value passed to PostQuitMessage
	return msg.wParam;
}

void App::Tick()
{
	const float c = sin( timer.Peek() ) / 2.f + 0.5f;
	wnd.Gfx().ClearBuffer( c, c, c );
	wnd.Gfx().DrawTriangle();
	wnd.Gfx().EndFrame();
}