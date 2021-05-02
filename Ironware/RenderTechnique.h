/*!
 * \file RenderTechnique.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#pragma once

#include "RenderStep.h"

#include <vector>

/**
 * @brief 
*/
class RenderTechnique
{
public:
	void AddStep( RenderStep step ) noexcept { steps.push_back( std::move( step ) ); }
	void InitializeParentReferences( const class Drawable& parent ) noexcept;
	void Submit( class FrameExecutor& frame, const class Drawable& drawable ) const noexcept;
	void Activate() noexcept { active = true; }
	void Deactivate() noexcept { active = false; }

private:
	bool active = true;
	std::vector<RenderStep> steps;
};