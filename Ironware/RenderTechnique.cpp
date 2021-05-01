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

void RenderTechnique::InitializeParentReferences( const Drawable & parent ) noexcept
{
	for( auto& s : steps )
	{
		s.InitializeParentReferences( parent );
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