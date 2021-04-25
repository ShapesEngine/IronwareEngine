/*!
 * \file App.cpp
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#include "App.h"
#define IR_INCLUDE_TEXTURE
#include "BindableCommon.h"
#include "BindableCollection.h"

App::App()
{
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.f, 9.f / 16.f, 0.5f, 400.f ) );
	wnd.EnableMouseCursor();
}

int App::BeginFrame()
{
	while( true )
	{
		// process all messages pending
		// if return optional has value, it means we're quitting
		if( const auto ecode = Window::ProcessMessages() )
		{
			// returns exit code
			return *ecode;
		}

		HandleInput();
		ProcessFrame();
	}
}

void App::ProcessFrame()
{
	wnd.Gfx().BeginFrame( 0.07f, 0.f, 0.12f );
	// move away by 20.f from origin
	wnd.Gfx().SetCamera( camera.GetMatrix() );
	pointLight.Bind( wnd.Gfx(), camera.GetMatrix() );

	pointLight.Draw( wnd.Gfx() );
	sponza.Draw( wnd.Gfx() );
	/*goblin.Draw( wnd.Gfx() );
	nano.Draw( wnd.Gfx() );
	wall.Draw( wnd.Gfx() );
	wallObj.Draw( wnd.Gfx() );*/

	// imgui window to control camera & light
	camera.SpawnControlWindow();
	pointLight.SpawnControlWindow();
	sponza.ShowWindow( wnd.Gfx(), "sponza" );
	/*goblin.ShowWindow( wnd.Gfx(), "Goblin" );
	nano.ShowWindow( wnd.Gfx(), "Nanosuit" );
	wallObj.ShowWindow( wnd.Gfx(), "Brickwall" );
	wall.SpawnControlWindow( wnd.Gfx() );*/

	// present frame
	wnd.Gfx().EndFrame();
}

void App::HandleInput()
{
	const float dt = timer.Mark();

	if( wnd.mouse.RightIsPressed() )
	{
		wnd.DisableMouseCursor();
	}
	else
	{
		wnd.EnableMouseCursor();
	}

	if( wnd.kbd.KeyIsPressed( VK_ESCAPE ) )
	{
		PostQuitMessage( 0 );
	}

	if( !wnd.IsCursorEnabled() )
	{
		if( wnd.kbd.KeyIsPressed( 'W' ) || wnd.kbd.KeyIsPressed( VK_UP ) )
		{
			camera.Translate( { 0.f, 0.f, dt } );
		}
		if( wnd.kbd.KeyIsPressed( 'S' ) || wnd.kbd.KeyIsPressed( VK_DOWN ) )
		{
			camera.Translate( { 0.f, 0.f, -dt } );
		}
		if( wnd.kbd.KeyIsPressed( 'D' ) || wnd.kbd.KeyIsPressed( VK_RIGHT ) )
		{
			camera.Translate( { dt, 0.f, 0.f } );
		}
		if( wnd.kbd.KeyIsPressed( 'A' ) || wnd.kbd.KeyIsPressed( VK_LEFT ) )
		{
			camera.Translate( { -dt, 0.f, 0.f } );
		}
		if( wnd.kbd.KeyIsPressed( 'E' ) || wnd.kbd.KeyIsPressed( VK_SPACE ) )
		{
			camera.Translate( { 0.f, dt, 0.f } );
		}
		if( wnd.kbd.KeyIsPressed( 'Q' ) || wnd.kbd.KeyIsPressed( VK_CONTROL ) )
		{
			camera.Translate( { 0.f, -dt, 0.f } );
		}

		while( const auto e = wnd.mouse.Read() )
		{
			if( e->GetType() == Mouse::Event::Type::RAWMOVE )
			{
				camera.Rotate( (float)e->GetRawDeltaX(), (float)e->GetRawDeltaY() );
			}
			else if( e->GetType() == Mouse::Event::Type::WHEELUP )
			{
				camera.SpeedUp();
			}
			else if( e->GetType() == Mouse::Event::Type::WHEELDOWN )
			{
				camera.SpeedDown();
			}
		}
	}
}