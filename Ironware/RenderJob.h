/*!
 * \file RenderJob.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "CommonMacros.h"

 /**
  * @brief
 */
class RenderJob
{
public:
	RenderJob( const class RenderStep* pStep, const class Drawable* pDrawable );
	void Execute( class Graphics& gfx ) const IFNOEXCEPT;

private:
	const Drawable* pDrawable;
	const RenderStep* pStep;
};

