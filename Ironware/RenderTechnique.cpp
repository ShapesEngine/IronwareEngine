/*!
 * \file RenderTechnique.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "RenderTechnique.h"
#include "FrameExecutor.h"

RenderTechnique::RenderTechnique( std::wstring name ) noexcept :
	name( name )
{
}

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

void RenderTechnique::Submit( FrameExecutor & frame, const Drawable & drawable ) const noexcept
{
	if( active )
	{
		for( const auto& step : steps )
		{
			step.Submit( frame, drawable );
		}
	}
}