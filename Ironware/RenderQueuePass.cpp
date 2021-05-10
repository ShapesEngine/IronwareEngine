/*!
 * \file RenderQueuePass.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#include "RenderQueuePass.h"

void RenderQueuePass::Accept( Job job ) noexcept
{
	jobs.push_back( job );
}

void RenderQueuePass::Execute( Graphics& gfx ) const IFNOEXCEPT
{
	BindAll( gfx );

	for( const auto& j : jobs )
	{
		j.Execute( gfx );
	}
}

void RenderQueuePass::Reset() IFNOEXCEPT
{
	jobs.clear();
}