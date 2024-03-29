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

class RenderGraph;

/**
 * @brief Class that holds 1 or more steps of object rendering
 * * and able of submitting rendering steps
*/
class RenderTechnique
{
public:
	RenderTechnique( size_t channelFilter );
	RenderTechnique( std::wstring name, size_t channelFilter, bool startActive = true ) noexcept;
	void InitializeParentReferences( const class Drawable& parent ) noexcept;
	void Accept( TechniqueProbe& probe );
	void Link( RenderGraph& rg );
	void AddStep( RenderStep step ) noexcept { steps.push_back( std::move( step ) ); }
	void Submit( const Drawable& drawable, size_t channelFilter ) const noexcept;
	bool IsActive() const noexcept { return active; }
	void SetActive( bool active_val ) noexcept { active = active_val; }
	const std::wstring& GetName() const noexcept { return name; }

private:
	bool active = true;
	std::vector<RenderStep> steps;
	std::wstring name;
	size_t channels;
};