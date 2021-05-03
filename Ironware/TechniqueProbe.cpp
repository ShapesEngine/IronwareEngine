/*!
 * \file TechniqueProbe.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#include "TechniqueProbe.h"
#include "RenderTechnique.h"
#include "RenderStep.h"

void TechniqueProbe::SetTechnique( RenderTechnique * pTech_in )
{
	pTech = pTech_in;
	OnSetTechnique();
}

void TechniqueProbe::SetStep( RenderStep * pStep_in )
{
	pStep = pStep_in;
	OnSetStep();
}