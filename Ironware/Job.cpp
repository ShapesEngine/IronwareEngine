/*!
 * \file RenderJob.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "Job.h"
#include "Graphics.h"
#include "Drawable.h"
#include "RenderStep.h"

Job::Job( const RenderStep* pStep, const Drawable * pDrawable ) :
	pStep( pStep ),
	pDrawable( pDrawable )
{}

void Job::Execute( Graphics & gfx ) const IFNOEXCEPT
{
	pDrawable->Bind( gfx );
	pStep->Bind( gfx );
	gfx.DrawIndexed( pDrawable->GetIndexCount() );
}
