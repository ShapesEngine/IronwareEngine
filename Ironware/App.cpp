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
#include "IronMath.h"

#include <DirectXTex/DirectXTex.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <string>

App::App()
{
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.f, 9.f / 16.f, 0.5f, 400.f ) );
	wnd.EnableMouseCursor();
	//auto camPos = camera.GetPos();
	/*std::wstring path = L"Models\\brickwall\\brickwall.obj";;
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile( to_narrow( path ),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);
	Material mat{ wnd.Gfx(),*pScene->mMaterials[1],path };*/
	//pLoaded = std::make_unique<Mesh>( wnd.Gfx(), mat, *pScene->mMeshes[0] );
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

	//goblin.Submit( fexe );
	pointLight.Submit( fexe );
	sponza.Submit( fexe );
	//pLoaded->Submit( fexe, DirectX::XMMatrixIdentity() );
	fexe.Execute( wnd.Gfx() );
	/*sheet1.Draw( wnd.Gfx() );
	sheet2.Draw( wnd.Gfx() );*/

	class TP : public TechniqueProbe
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
			if( auto v = buf["offset"]; v.Exists() )
			{
				dcheck( ImGui::SliderFloat( tag( "Offset" ), &v, 0.0f, 1.0f, "%.3f", 2.5f ) );
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
			if( auto v = buf["useSpecMap"]; v.Exists() )
			{
				dcheck( ImGui::Checkbox( tag( "Spec. Map Enable" ), &v ) );
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
	};

	class MP : ModelProbe
	{
	public:
		void SpawnWindow( Model& model )
		{
			ImGui::Begin( "Model" );
			ImGui::Columns( 2, nullptr, true );
			model.Accept( *this );

			ImGui::NextColumn();
			if( pSelectedNode )
			{
				bool dirty = false;
				const auto dcheck = [&dirty]( bool changed ) {dirty = dirty || changed; };
				auto& tf = ResolveTransform();
				ImGui::TextColored( { 0.4f,1.0f,0.6f,1.0f }, "Translation" );
				dcheck( ImGui::SliderFloat( "X", &tf.x, -60.f, 60.f ) );
				dcheck( ImGui::SliderFloat( "Y", &tf.y, -60.f, 60.f ) );
				dcheck( ImGui::SliderFloat( "Z", &tf.z, -60.f, 60.f ) );
				ImGui::TextColored( { 0.4f,1.0f,0.6f,1.0f }, "Orientation" );
				dcheck( ImGui::SliderAngle( "X-rotation", &tf.xRot, -180.0f, 180.0f ) );
				dcheck( ImGui::SliderAngle( "Y-rotation", &tf.yRot, -180.0f, 180.0f ) );
				dcheck( ImGui::SliderAngle( "Z-rotation", &tf.zRot, -180.0f, 180.0f ) );
				if( dirty )
				{
					pSelectedNode->SetAppliedTransform(
						dx::XMMatrixRotationX( tf.xRot ) *
						dx::XMMatrixRotationY( tf.yRot ) *
						dx::XMMatrixRotationZ( tf.zRot ) *
						dx::XMMatrixTranslation( tf.x, tf.y, tf.z )
					);
				}
			}
			if( pSelectedNode )
			{
				TP probe;
				pSelectedNode->Accept( probe );
			}
			ImGui::End();
		}

	protected:
		bool PushNode( Node& node ) override
		{
			const auto index = node.GetID();
			const auto node_flags =
				( pSelectedNode != nullptr && index == pSelectedNode->GetID() ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None ) |
				( node.HasChildren() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow );

			const bool isExpanded = ImGui::TreeNodeEx( reinterpret_cast<void*>( (intptr_t)index ), node_flags, node.GetName().c_str() );
			if( ImGui::IsItemClicked() )
			{
				// used to change the highlighted node on selection change
				struct Probe : public TechniqueProbe
				{
					virtual void OnSetTechnique()
					{
						if( pTech->GetName() == L"Outline" )
						{
							pTech->SetActive( highlighted );
						}
					}
					bool highlighted = false;
				} probe;

				// remove highlight on prev-selected node
				if( pSelectedNode != nullptr )
				{
					pSelectedNode->Accept( probe );
				}
				// add highlight to newly-selected node
				probe.highlighted = true;
				node.Accept( probe );
				pSelectedNode = &node;
			}
			return isExpanded;
		}

		void PopNode( Node& node ) override { ImGui::TreePop(); }

	private:
		Node* pSelectedNode = nullptr;
		struct TransformParameters
		{
			float xRot = 0.0f;
			float yRot = 0.0f;
			float zRot = 0.0f;
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
		};
		std::unordered_map<int, TransformParameters> transformParams;
	private:
		TransformParameters& ResolveTransform() noexcept
		{
			const auto id = pSelectedNode->GetID();
			auto i = transformParams.find( id );
			if( i == transformParams.end() )
			{
				return LoadTransform( id );
			}
			return i->second;
		}
		TransformParameters& LoadTransform( int id ) noexcept
		{
			const auto& applied = pSelectedNode->GetAppliedTransform();
			const auto angles = extract_euler_angles( applied );
			const auto translation = extract_translation( applied );
			TransformParameters tp;
			tp.zRot = angles.z;
			tp.xRot = angles.x;
			tp.yRot = angles.y;
			tp.x = translation.x;
			tp.y = translation.y;
			tp.z = translation.z;
			return transformParams.insert( { id,{ tp } } ).first->second;
		}
	};

	static MP modelProbe;

	// imgui window to control camera & light
	modelProbe.SpawnWindow( sponza );
	camera.SpawnControlWindow();
	pointLight.SpawnControlWindow();

	//sponza.ShowWindow( wnd.Gfx(), "sponza" );
	/*sheet1.SpawnControlWindow( wnd.Gfx(), "sheet1" );
	sheet2.SpawnControlWindow( wnd.Gfx(), "sheet2" );*/
	/*goblin.ShowWindow( wnd.Gfx(), "Goblin" );
	nano.ShowWindow( wnd.Gfx(), "Nanosuit" );
	wallObj.ShowWindow( wnd.Gfx(), "Brickwall" );
	wall.SpawnControlWindow( wnd.Gfx() );*/

	//box1.SpawnControlWindow( wnd.Gfx(), "box1" );
	//box2.SpawnControlWindow( wnd.Gfx(), "box2" );

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