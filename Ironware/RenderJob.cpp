/*!
 * \file RenderJob.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "RenderJob.h"
#include "Graphics.h"
#include "Drawable.h"
#include "RenderStep.h"

RenderJob::RenderJob( const RenderStep* pStep, const Drawable * pDrawable ) :
	pStep( pStep ),
	pDrawable( pDrawable )
{}

void RenderJob::Execute( Graphics & gfx ) const noexcept( !IS_DEBUG )
{
	pDrawable->Bind( gfx );
	pStep->Bind( gfx );
	gfx.DrawIndexed( pDrawable->GetIndexCount() );
}
