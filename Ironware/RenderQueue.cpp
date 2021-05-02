/*!
 * \file RenderQueue.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#include "RenderQueue.h"

void RenderQueue::Execute( Graphics & gfx ) const IFNOEXCEPT
{
	for( const auto& j : jobs )
	{
		j.Execute( gfx );
	}
}
