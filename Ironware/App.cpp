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
#include "IronMath.h"
#include "IronMath.h"
#include "GDIPlusManager.h"
#include "Surface.h"
#include "imgui/imgui.h"

#include <memory>
#include <algorithm>
#include <iterator>
#include <sstream>

GDIPlusManager gdiplm;

App::App() :
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
			return std::make_unique<Box>(
				gfx, rng, adist, ddist,
				odist, rdist, bdist
				);
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}( ) };
		std::uniform_real_distribution<float> adist{ 0.f, PI * 2.f };
		std::uniform_real_distribution<float> ddist{ 0.f, PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.f, PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.f, 20.f };
		std::uniform_real_distribution<float> bdist{ 0.4f, 3.f };
	};

	drawables.reserve( MAX_NDRAWABLES );
	std::generate_n( std::back_inserter( drawables ), MAX_NDRAWABLES, Factory{ wnd.Gfx() } );
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.f, 3.f / 4.f, 0.5f, 40.f ) );
}

int App::Begin()
{
	while( true )
	{
		if( wnd.kbd.KeyIsPressed( VK_ESCAPE ) )
			PostQuitMessage( 0 );

		// process all messages pending, but to not block for new messages
		if( const auto ecode = Window::ProcessMessages() )
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		SetupFrame();
	}
}

void App::SetupFrame()
{
	const auto dt = timer.Mark() * simulation_speed_factor;
	wnd.Gfx().BeginFrame( 0.07f, 0.f, 0.12f );
	// move away by 20.f from origin
	wnd.Gfx().SetCamera( camera.GetMatrix() );
	pointLight.Bind( wnd.Gfx() );

	for( auto& d : drawables )
	{
		d->Update( isSimulationRunning ? dt : 0.f );
		d->Draw( wnd.Gfx() );
	}
	pointLight.Draw( wnd.Gfx() );

	static char buffer[1024];
	// imgui window to control simulation speed
	if( ImGui::Begin( "Simulation Speed" ) )
	{
		std::ostringstream simulationStatusText;
		simulationStatusText << "Simulation State: " << ( isSimulationRunning ? "Running." : "Stopped." ) <<  "\nYou can hold Space Bar to Stop!";
		const float frame_rate = ImGui::GetIO().Framerate;
		ImGui::SliderFloat( "Speed Factor", &simulation_speed_factor, 0.f, 5.f );
		ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.f / frame_rate, frame_rate );
		ImGui::Text( simulationStatusText.str().c_str() );
	}
	if( wnd.kbd.KeyIsPressed( VK_SPACE ) || simulation_speed_factor == 0.f )
	{
		isSimulationRunning = false;
	}
	else
	{
		isSimulationRunning = true;
	}
		
	ImGui::End();
	// imgui window to control camera & light
	camera.SpawnControlWindow();
	pointLight.SpawnControlWindow();

	// present frame
	wnd.Gfx().EndFrame();
}