/*!
 * \file StepLinkingProbe.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#pragma once

#include "TechniqueProbe.h"

class RenderGraph;

class StepLinkingProbe : public TechniqueProbe
{
protected:
	void OnSetStep() override;

private:
	RenderGraph& rg;
};