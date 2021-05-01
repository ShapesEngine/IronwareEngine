/*!
 * \file RenderQueue.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#include "RenderQueue.h"

void RenderQueue::Execute( Graphics & gfx ) const noexcept( !IS_DEBUG )
{
	for( const auto& j : jobs )
	{
		j.Execute( gfx );
	}
}
