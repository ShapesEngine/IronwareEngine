/*!
 * \file App.cpp
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#include "App.h"

App::App() :
	pointLight( wnd.Gfx() )
{
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.f, 9.f / 16.f, 0.5f, 40.f ) );
	wnd.DisableMouseCursor();
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

	nano.Draw( wnd.Gfx() );
	pointLight.Draw( wnd.Gfx() );

	// imgui window to control camera & light
	camera.SpawnControlWindow();
	pointLight.SpawnControlWindow();

	ShowRawMouseWindow();
	nano.ShowWindow( "Nanosuit" );

	// present frame
	wnd.Gfx().EndFrame();
}

void App::HandleInput()
{
	if( !wnd.kbd.KeyIsEmpty() )
	{
		const auto e = wnd.kbd.ReadKey();
		if( e->GetCode() == VK_ESCAPE )
		{
			PostQuitMessage( 0 );
		}
	}

	if( !wnd.mouse.IsEmpty() )
	{
		const auto e = wnd.mouse.Read();
		if( e->GetType() == Mouse::Event::Type::RPRESS )
		{
			ToggleCursor();
		}
	}
}



void App::ShowRawMouseWindow()
{
	/*while( const auto e = wnd.mouse.Read() )
	{
		if( e->GetType() == Mouse::Event::Type::RAWMOVE )
		{
			x += e->GetRawDeltaX();
			y += e->GetRawDeltaY();
		}
	}*/
	x = wnd.mouse.GetPosX();
	y = wnd.mouse.GetPosY();
	if( ImGui::Begin( "Raw Input" ) )
	{
		ImGui::Text( "Tally: (%d,%d)", x, y );
	}
	ImGui::End();
}