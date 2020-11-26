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

#include <memory>
#include <algorithm>
#include <iterator>

App::App() :
	wnd( 640, 480, L"Ironware Engine" )
{
	class Factory
	{
	public:
		Factory( Graphics& gfx )
			:
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
		std::uniform_int_distribution<int> typedist{ 0, 2 };
	};

	Factory f( wnd.Gfx() );
	drawables.reserve( nDrawables );
	std::generate_n( std::back_inserter( drawables ), nDrawables, f );

	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.f, 3.f / 4.f, 0.5f, 40.f ) );
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
	const auto dt = timer.Mark();
	wnd.Gfx().ClearBuffer( 0.07f, 0.f, 0.12f );
	for( auto& d : drawables )
	{
		d->Update( dt );
		d->Draw( wnd.Gfx() );
	}
	wnd.Gfx().EndFrame();
}