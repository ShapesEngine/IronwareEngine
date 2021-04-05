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
	wnd( 1280, 960, L"Ironware Engine" ),
	pointLight( wnd.Gfx() )
{
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.f, 3.f / 4.f, 0.5f, 40.f ) );
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

	// present frame
	wnd.Gfx().EndFrame();
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