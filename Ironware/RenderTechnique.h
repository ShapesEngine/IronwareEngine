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
#include "TechniqueProbe.h"

#include <vector>

 /**
  * @brief Class that holds 1 or more steps of object rendering
  * * and able of submitting rendering steps
 */
class RenderTechnique
{
public:
	RenderTechnique() = default;
	RenderTechnique( std::wstring name ) noexcept;
	void InitializeParentReferences( const class Drawable& parent ) noexcept;
	void Accept( TechniqueProbe& probe );
	void AddStep( RenderStep step ) noexcept { steps.push_back( std::move( step ) ); }
	void Submit( class FrameExecutor& frame, const class Drawable& drawable ) const noexcept;
	bool IsActive() const noexcept { return active; }
	void SetActive( bool active_val ) noexcept { active = active_val; }
	const std::wstring& GetName() const noexcept { return name; }

private:
	bool active = true;
	std::vector<RenderStep> steps;
	std::wstring name = L"?";
};