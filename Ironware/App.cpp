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
#include "Melon.h"
#include "Pyramid.h"
#include "IronMath.h"
#include "GDIPlusManager.h"
#include "Surface.h"
#include "Sheet.h"
#include "SkinnedBox.h"
#include "imgui/imgui.h"

#include <memory>
#include <algorithm>
#include <iterator>

GDIPlusManager gdiplm;

App::App() :
	wnd( 640, 480, L"Ironware" )
{
	class Factory
	{
	public:
		Factory( Graphics& gfx ) :
			gfx( gfx )
		{}
		std::unique_ptr<Drawable> operator()()
		{
			switch( typedist( rng ) )
			{
			case 0:
				return std::make_unique<Pyramid>(
					gfx, rng, adist, ddist,
					odist, rdist
					);
			case 1:
				return std::make_unique<Box>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
					);
			case 2:
				return std::make_unique<Melon>(
					gfx, rng, adist, ddist,
					odist, rdist, longdist, latdist
					);
			case 3:
				return std::make_unique<Sheet>(
					gfx, rng, adist, ddist,
					odist, rdist
					);
			case 4:
				return std::make_unique<SkinnedBox>(
					gfx, rng, adist, ddist,
					odist, rdist
					);
			default:
				assert( false && "bad drawable type in factory" );
				return {};
			}
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}( ) };
		std::uniform_real_distribution<float> adist{ 0.f, PI * 2.f };
		std::uniform_real_distribution<float> ddist{ 0.f, PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.f, PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.f, 20.f };
		std::uniform_real_distribution<float> bdist{ 0.4f, 3.f };
		std::uniform_int_distribution<int> latdist{ 5, 20 };
		std::uniform_int_distribution<int> longdist{ 10, 40 };
		std::uniform_int_distribution<int> typedist{ 0, 4 };
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

	for( auto& d : drawables )
	{
		d->Update( wnd.kbd.KeyIsPressed( VK_SPACE ) ? 0.f : dt );
		d->Draw( wnd.Gfx() );
	}

	static char buffer[1024];
	// imgui window to control simulation speed
	if( ImGui::Begin( "Simulation Speed" ) )
	{
		const float frame_rate = ImGui::GetIO().Framerate;
		ImGui::SliderFloat( "Speed Factor", &simulation_speed_factor, 0.f, 5.f );
		ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.f / frame_rate, frame_rate );
		ImGui::InputText( "Text IR", buffer, sizeof( buffer ) );
	}
	ImGui::End();

	// present frame
	wnd.Gfx().EndFrame();
}