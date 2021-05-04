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

#include <DirectXTex/DirectXTex.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <string>

App::App()
{
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.f, 9.f / 16.f, 0.5f, 400.f ) );
	wnd.EnableMouseCursor();
	auto camPos = camera.GetPos();
	std::wstring path = L"Models\\brickwall\\brickwall.obj";;
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile( to_narrow( path ),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);
	Material mat{ wnd.Gfx(),*pScene->mMaterials[1],path };
	pLoaded = std::make_unique<Mesh>( wnd.Gfx(), mat, *pScene->mMeshes[0] );
	/*sheet1.SetPos( camPos );
	sheet2.SetPos( { camPos.x, camPos.y, camPos.z - 5.f } );*/
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

	pointLight.Submit( fexe );
	//sponza.Draw( wnd.Gfx() );
	/*box1.Submit( fexe );
	box2.Submit( fexe );*/
	pLoaded->Submit( fexe, DirectX::XMMatrixIdentity() );
	fexe.Execute( wnd.Gfx() );
	/*sheet1.Draw( wnd.Gfx() );
	sheet2.Draw( wnd.Gfx() );*/

	class Probe : public TechniqueProbe
	{
	public:
		void OnSetTechnique() override
		{
			using namespace std::string_literals;
			ImGui::TextColored( { 0.4f,1.0f,0.6f,1.0f }, to_narrow( pTech->GetName() ).c_str() );
			bool active = pTech->IsActive();
			ImGui::Checkbox( ( "Tech Active##"s + std::to_string( techIdx ) ).c_str(), &active );
			pTech->SetActive( active );
		}
		bool OnVisitBuffer( Buffer& buf ) override
		{
			namespace dx = DirectX;
			float dirty = false;
			const auto dcheck = [&dirty]( bool changed ) {dirty = dirty || changed; };
			auto tag = [tagScratch = std::string{}, tagString = "##" + std::to_string( bufIdx )]
			( const char* label ) mutable
			{
				tagScratch = label + tagString;
				return tagScratch.c_str();
			};

			if( auto v = buf["scale"]; v.Exists() )
			{
				dcheck( ImGui::SliderFloat( tag( "Scale" ), &v, 1.0f, 2.0f, "%.3f", 3.5f ) );
			}
			if( auto v = buf["materialColor"]; v.Exists() )
			{
				dcheck( ImGui::ColorPicker3( tag( "Color" ), reinterpret_cast<float*>( &static_cast<dx::XMFLOAT3&>( v ) ) ) );
			}
			if( auto v = buf["specularColor"]; v.Exists() )
			{
				dcheck( ImGui::ColorPicker3( tag( "Spec. Color" ), reinterpret_cast<float*>( &static_cast<dx::XMFLOAT3&>( v ) ) ) );
			}
			if( auto v = buf["specularGloss"]; v.Exists() )
			{
				dcheck( ImGui::SliderFloat( tag( "Glossiness" ), &v, 1.0f, 100.0f, "%.1f", 1.5f ) );
			}
			if( auto v = buf["specularWeight"]; v.Exists() )
			{
				dcheck( ImGui::SliderFloat( tag( "Spec. Weight" ), &v, 0.0f, 2.0f ) );
			}
			if( auto v = buf["useNormalMap"]; v.Exists() )
			{
				dcheck( ImGui::Checkbox( tag( "Normal Map Enable" ), &v ) );
			}
			if( auto v = buf["normalMapWeight"]; v.Exists() )
			{
				dcheck( ImGui::SliderFloat( tag( "Normal Map Weight" ), &v, 0.0f, 2.0f ) );
			}
			return dirty;
		}
	} probe;
	pLoaded->Accept( probe );

	// imgui window to control camera & light
	camera.SpawnControlWindow();
	pointLight.SpawnControlWindow();
	
	//sponza.ShowWindow( wnd.Gfx(), "sponza" );
	/*sheet1.SpawnControlWindow( wnd.Gfx(), "sheet1" );
	sheet2.SpawnControlWindow( wnd.Gfx(), "sheet2" );*/
	/*goblin.ShowWindow( wnd.Gfx(), "Goblin" );
	nano.ShowWindow( wnd.Gfx(), "Nanosuit" );
	wallObj.ShowWindow( wnd.Gfx(), "Brickwall" );
	wall.SpawnControlWindow( wnd.Gfx() );*/

	box1.SpawnControlWindow( wnd.Gfx(), "box1" );
	box2.SpawnControlWindow( wnd.Gfx(), "box2" );

	// present frame
	wnd.Gfx().EndFrame();
	fexe.Reset();
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