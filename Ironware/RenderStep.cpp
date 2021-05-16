/*!
 * \file RenderStep.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "RenderStep.h"
#include "Drawable.h"
#include "RenderQueuePass.h"
#include "RenderGraph.h"

RenderStep::RenderStep( std::string targetPassName ) :
	targetPassName{ std::move( targetPassName ) }
{}

RenderStep::RenderStep( const RenderStep & src ) noexcept :
	targetPassName( src.targetPassName )
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

void RenderStep::Submit( const Drawable & drawable ) const
{
	pTargetPass->Accept( Job{ this, &drawable } );
}

void RenderStep::Bind( Graphics & gfx ) const IFNOEXCEPT
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

void RenderStep::Link( RenderGraph & rg )
{
	assert( !pTargetPass );
	pTargetPass = &rg.GetRenderQueue( targetPassName );
}