/*!
 * \file TechniqueProbe.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#pragma once

/**
 * @brief Class that goes through meshes and finds appropriate buffer
*/
class TechniqueProbe
{
public:
	void SetTechnique( class RenderTechnique* pTech_in );	
	void SetStep( class RenderStep* pStep_in );
	
	virtual bool VisitBuffer( class Buffer& ) = 0;

protected:
	virtual void OnSetTechnique() {}
	virtual void OnSetStep() {}

protected:
	class RenderTechnique* pTech = nullptr;
	class RenderStep* pStep = nullptr;
};