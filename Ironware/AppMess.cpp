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
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
#include "AppMess.h"
#include "WindowExceptionMacros.h"
#include "IronMath.h"
#include "IronMath.h"
#include "GDIPlusManager.h"
#include "Box.h"
#include "TexturedBox.h"
#include "Surface.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Model.h"

#include <imgui/imgui.h>

#include <memory>
#include <algorithm>
#include <iterator>
#include <sstream>

GDIPlusManager gdiplm;

AppMess::AppMess() :
	wnd( 1024, 768, L"Ironware" ),
	pointLight( wnd.Gfx() )
{
	class Factory
	{
	public:
		Factory( Graphics& gfx ) :
			gfx( gfx )
		{}
		std::unique_ptr<Drawable> operator()()
		{
			switch( const DirectX::XMFLOAT3 matCol = { cdist( rng ), cdist( rng ), cdist( rng ) }; sdist( rng ) )
			{
			case 0:
				return std::make_unique<Box>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist, matCol
					);
			case 1:
				return std::make_unique<Cylinder>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist, tdist
					);
			case 2:
				return std::make_unique<TexturedBox>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
					);
			case 3:
				return std::make_unique<Model>(
					gfx, rng, adist, ddist,
					odist, rdist, DirectX::XMFLOAT3{ cdist( rng ), cdist( rng ), cdist( rng ) },
					1.f
					);
			default:
				assert( false && "impossible drawable option in factory" );
				return {};
			}
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}( ) };
		std::uniform_int_distribution<> sdist{ 0, 3 };
		std::uniform_real_distribution<float> adist{ 0.f, PI * 2.f };
		std::uniform_real_distribution<float> ddist{ 0.f, PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.f, PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.f, 20.f };
		std::uniform_real_distribution<float> bdist{ 0.4f, 3.f };
		std::uniform_real_distribution<float> cdist{ 0.f, 1.f };
		std::uniform_int_distribution<> tdist{ 3, 30 };
	};

	drawables.reserve( MAX_NDRAWABLES );
	std::generate_n( std::back_inserter( drawables ), MAX_NDRAWABLES, Factory{ wnd.Gfx() } );
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.f, 3.f / 4.f, 0.5f, 40.f ) );

	for( auto& pD : drawables )
	{
		if( auto pB = dynamic_cast<Box*>( pD.get() ) )
		{
			boxes.push_back( pB );
		}
	}
}

int AppMess::BeginFrame()
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

void AppMess::ProcessFrame()
{
	if( wnd.kbd.KeyIsPressed( VK_ESCAPE ) )
		PostQuitMessage( 0 );

	const float mark = timer.Mark();

	const auto dt = isSimulationRunning ? mark * simulationSpeedFactor : 0.f;
	wnd.Gfx().BeginFrame( 0.07f, 0.f, 0.12f );
	// move away by 20.f from origin
	wnd.Gfx().SetCamera( camera.GetMatrix() );
	pointLight.Bind( wnd.Gfx(), camera.GetMatrix() );

	for( auto& d : drawables )
	{
		d->Update( dt );
		d->Draw( wnd.Gfx() );
	}
	pointLight.Draw( wnd.Gfx() );

	SpawnBoxWindowManagerWindow();
	SpawnActiveBoxWindows();

	SpawnSimulationWindow();

	// toggling simulation
	if( !wnd.kbd.KeyIsEmpty() )
	{
		const auto k = wnd.kbd.ReadKey();
		if( ( k->IsPress() && k->GetCode() == VK_SPACE ) )
		{
			isSimulationRunning = !isSimulationRunning;
		}
	}

	// TODO: fix resuming simulation after the speedfactor was 0
	if( simulationSpeedFactor == 0.f )
	{
		isSimulationRunning = false;
	}

	ImGui::End();
	// imgui window to control camera & light
	camera.SpawnControlWindow();
	pointLight.SpawnControlWindow();

	// present frame
	wnd.Gfx().EndFrame();
}

void AppMess::SpawnActiveBoxWindows()
{
	// imgui box attribute control windows
	for( auto i = boxControlIndexes.begin(); i != boxControlIndexes.end(); )
	{
		if( !boxes[*i]->SpawnControlWindow( *i, wnd.Gfx() ) )
		{
			i = boxControlIndexes.erase( i );
		}
		else
		{
			i++;
		}
	}
}

void AppMess::SpawnSimulationWindow()
{
	// imgui window to control simulation speed
	if( ImGui::Begin( "Simulation Speed" ) )
	{
		std::ostringstream simulationStatusText;
		simulationStatusText << "Simulation State: " << ( isSimulationRunning ? "Running." : "Stopped." ) << "\nYou can press Space Bar to Stop!";
		const float frame_rate = ImGui::GetIO().Framerate;
		ImGui::SliderFloat( "Speed Factor", &simulationSpeedFactor, 0.f, 5.f, "%.4f", 3.2f );
		ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.f / frame_rate, frame_rate );
		ImGui::Text( "Application running for: %.f seconds", timer.Peek() );
		ImGui::Text( simulationStatusText.str().c_str() );
	}
}

void AppMess::SpawnBoxWindowManagerWindow()
{
	if( ImGui::Begin( "Boxes" ) )
	{
		const auto preview = comboBoxIndex ? std::to_string( *comboBoxIndex ) : "Choose a box...";
		if( ImGui::BeginCombo( "Box Number", preview.c_str() ) )
		{
			for( int i = 0; i < boxes.size(); i++ )
			{
				const bool selected = *comboBoxIndex == i;
				if( ImGui::Selectable( std::to_string( i ).c_str(), selected ) )
				{
					comboBoxIndex = i;
				}
				if( selected )
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		if( ImGui::Button( "Spawn Control Window" ) && comboBoxIndex )
		{
			boxControlIndexes.insert( *comboBoxIndex );
			comboBoxIndex.reset();
		}
	}
	ImGui::End();
}
