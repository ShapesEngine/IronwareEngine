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
#include "WindowExceptionMacros.h"

App::App() :
	wnd( 640, 480, L"Ironware Engine" )
{}

int32_t App::Begin()
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
		SetupFrame();
	}

	// check if GetMessage call itself worked
	if( gResult == -1 )
	{
		throw IRWND_LAST_EXCEPT();
	}

	// wParam here is the value passed to PostQuitMessage
	return (int32_t)msg.wParam;
}

void App::SetupFrame()
{
	const float c = sin( timer.Peek() ) / 2.f + 0.5f;
	wnd.Gfx().ClearBuffer( c, c, c );
	wnd.Gfx().DrawTriangle(
		-timer.Peek(),
		0.f,
		0.f
	);
	wnd.Gfx().DrawTriangle( 
		timer.Peek(), 
		float(wnd.mouse.GetPosX() ) / float( wnd.GetWidth() / 2) - 1.f, 
		-float(wnd.mouse.GetPosY() ) / float(wnd.GetHeight() / 2) + 1.f 
	);
	wnd.Gfx().EndFrame();
}