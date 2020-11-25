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

#include <memory>

App::App() :
	wnd( 640, 480, L"Ironware Engine" )
{
	std::mt19937 rng( std::random_device{}( ) );
	std::uniform_real_distribution<float> adist( 0.0f, 3.1415f * 2.0f );
	std::uniform_real_distribution<float> ddist( 0.0f, 3.1415f * 2.0f );
	std::uniform_real_distribution<float> odist( 0.0f, 3.1415f * 0.3f );
	std::uniform_real_distribution<float> rdist( 6.0f, 20.0f );
	for( auto i = 0; i < 1; i++ )
	{
		boxes.push_back( std::make_unique<Box>(
			wnd.Gfx(), rng, adist,
			ddist, odist, rdist
			) );
	}
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 3.0f / 4.0f, 0.5f, 40.0f ) );
}

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
		SetupFrame();
	}

	// check if GetMessage call itself worked
	if( gResult == -1 )
	{
		throw IRWND_LAST_EXCEPT();
	}

	// wParam here is the value passed to PostQuitMessage
	return (int)msg.wParam;
}

void App::SetupFrame()
{
	const float c = sin( timer.Peek() ) / 2.f + 0.5f;
	wnd.Gfx().ClearBuffer( c, c, c );
	auto dt = timer.Mark();
	wnd.Gfx().ClearBuffer( 0.07f, 0.0f, 0.12f );
	for( auto& b : boxes )
	{
		b->Update( dt );
		b->Draw( wnd.Gfx() );
	}
	wnd.Gfx().EndFrame();
}