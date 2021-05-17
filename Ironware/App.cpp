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
#include "DynamicConstantBuffer.h"
#include "ModelProbe.h"
#include "Node.h"
#include "TestModelProbe.h"
#include "Camera.h"

#include <DirectXTex/DirectXTex.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <string>

App::App()
{
	cameras.AddCamera( std::make_unique<Camera>( wnd.Gfx(), "1", DirectX::XMFLOAT3{ -60.f, 5.f, 2.f }, 0.f, PI / 2.f ) );
	cameras.AddCamera( std::make_unique<Camera>( wnd.Gfx(), "2", DirectX::XMFLOAT3{ 60.f, 5.f, 2.f }, 0.f, -PI / 2.f ) );

	pointLight.LinkTechniques( rg );
	sponza.LinkTechniques( rg );
	goblin.LinkTechniques( rg );
	nano.LinkTechniques( rg );
	cube.LinkTechniques( rg );
	cube2.LinkTechniques( rg );
	cameras.LinkTechniques( rg );

	nano.SetRootTransform(
		DirectX::XMMatrixRotationY( PI / 2.f ) *
		DirectX::XMMatrixTranslation( 40.f, 0.f, 2.f )
	);
	cube.SetPos( { 40.f, 0.f, 25.f } );

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
	cameras->BindToGraphics( wnd.Gfx() );
	pointLight.Bind( wnd.Gfx(), cameras->GetMatrix() );

	nano.Submit();
	goblin.Submit();
	pointLight.Submit();
	sponza.Submit();
	cube.Submit();
	//cube2.Submit();
	cameras.Submit();

	rg.Execute( wnd.Gfx() );

	// imgui windows
	static MP sponzaProbe{ "Sponza" };
	static MP goblinProbe{ "Goblin" };
	static MP nanoProbe{ "Nanosuit" };
	sponzaProbe.SpawnWindow( sponza );
	nanoProbe.SpawnWindow( nano );
	goblinProbe.SpawnWindow( goblin );
	cameras.SpawnWindow( wnd.Gfx() );
	pointLight.SpawnControlWindow();

	rg.RenderWidgets( wnd.Gfx() );

	// present
	wnd.Gfx().EndFrame();
	rg.Reset();
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
			cameras->Translate( { 0.f, 0.f, dt } );
		}
		if( wnd.kbd.KeyIsPressed( 'S' ) || wnd.kbd.KeyIsPressed( VK_DOWN ) )
		{
			cameras->Translate( { 0.f, 0.f, -dt } );
		}
		if( wnd.kbd.KeyIsPressed( 'D' ) || wnd.kbd.KeyIsPressed( VK_RIGHT ) )
		{
			cameras->Translate( { dt, 0.f, 0.f } );
		}
		if( wnd.kbd.KeyIsPressed( 'A' ) || wnd.kbd.KeyIsPressed( VK_LEFT ) )
		{
			cameras->Translate( { -dt, 0.f, 0.f } );
		}
		if( wnd.kbd.KeyIsPressed( 'E' ) || wnd.kbd.KeyIsPressed( VK_SPACE ) )
		{
			cameras->Translate( { 0.f, dt, 0.f } );
		}
		if( wnd.kbd.KeyIsPressed( 'Q' ) || wnd.kbd.KeyIsPressed( VK_CONTROL ) )
		{
			cameras->Translate( { 0.f, -dt, 0.f } );
		}

		while( const auto e = wnd.mouse.Read() )
		{
			if( e->GetType() == Mouse::Event::Type::RAWMOVE )
			{
				cameras->Rotate( (float)e->GetRawDeltaX(), (float)e->GetRawDeltaY() );
			}
			else if( e->GetType() == Mouse::Event::Type::WHEELUP )
			{
				cameras->SpeedUp();
			}
			else if( e->GetType() == Mouse::Event::Type::WHEELDOWN )
			{
				cameras->SpeedDown();
			}
		}
	}
}