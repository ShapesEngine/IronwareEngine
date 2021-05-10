/*!
 * \file RenderTechnique.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "RenderTechnique.h"
#include "RenderGraph.h"

RenderTechnique::RenderTechnique( std::wstring name, bool startActive ) noexcept :
	name( name ),
	active( startActive )
{}

void RenderTechnique::InitializeParentReferences( const Drawable & parent ) noexcept
{
	for( auto& s : steps )
	{
		s.InitializeParentReferences( parent );
	}
}

void RenderTechnique::Accept( TechniqueProbe & probe )
{
	probe.SetTechnique( this );
	for( auto& s : steps )
	{
		s.Accept( probe );
	}
}

void RenderTechnique::Submit( const Drawable & drawable ) const noexcept
{
	if( active )
	{
		for( const auto& step : steps )
		{
			step.Submit( drawable );
		}
	}
}

void RenderTechnique::Link( RenderGraph& rg )
{
	for( auto& step : steps )
	{
		step.Link( rg );
	}
}