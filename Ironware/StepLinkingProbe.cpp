/*!
 * \file StepLinkingProbe.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#include "StepLinkingProbe.h"
#include "RenderGraph.h"
#include "RenderStep.h"

void StepLinkingProbe::OnSetStep()
{
	pStep->Link( rg );
}