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
	techIdx++;
	OnSetTechnique();
}

void TechniqueProbe::SetStep( RenderStep * pStep_in )
{
	pStep = pStep_in;
	stepIdx++;
	OnSetStep();
}

bool TechniqueProbe::VisitBuffer( Buffer & buf )
{
	bufIdx++;
	return OnVisitBuffer( buf );
}