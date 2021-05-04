/*!
 * \file RenderStep.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "RenderStep.h"
#include "FrameExecutor.h"

RenderStep::RenderStep( size_t targetPass_in ) :
	targetPass{ targetPass_in }
{}

RenderStep::RenderStep( const RenderStep & src ) noexcept :
	targetPass( src.targetPass )
{
	bindables.reserve( src.bindables.size() );
	for( auto& pb : src.bindables )
	{
		if( auto* pCloning = dynamic_cast<const CloningBindable*>( pb.get() ) )
		{
			bindables.push_back( pCloning->Clone() );
		}
		else
		{
			bindables.push_back( pb );
		}
	}
}

void RenderStep::Submit( FrameExecutor & frame, const Drawable & drawable ) const
{
	frame.Accept( RenderJob{ this, &drawable }, targetPass );
}

void RenderStep::Bind( Graphics & gfx ) const
{
	for( const auto& b : bindables )
	{
		b->Bind( gfx );
	}
}

void RenderStep::InitializeParentReferences( const Drawable & parent ) noexcept
{
	for( auto& b : bindables )
	{
		b->InitializeParentReference( parent );
	}
}

void RenderStep::Accept( TechniqueProbe & probe )
{
	probe.SetStep( this );
	for( auto& pb : bindables )
	{
		pb->Accept( probe );
	}
}