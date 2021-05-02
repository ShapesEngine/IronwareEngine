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
  * @brief Class that is responsible for managing drawing objects
  * @note It's usually stored in RenderQueue container
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

