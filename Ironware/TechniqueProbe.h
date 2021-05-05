/*!
 * \file TechniqueProbe.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include <limits>

 /**
  * @brief Class that goes through meshes and finds appropriate buffer
 */
class TechniqueProbe
{
public:
	void SetTechnique( class RenderTechnique* pTech_in );
	void SetStep( class RenderStep* pStep_in );

	bool VisitBuffer( class Buffer& buf );

protected:
	virtual void OnSetTechnique() {}
	virtual void OnSetStep() {}
	virtual bool OnVisitBuffer( class Buffer& ) { return false; }

protected:
	class RenderTechnique* pTech = nullptr;
	class RenderStep* pStep = nullptr;
	size_t techIdx = std::numeric_limits<size_t>::max();
	size_t stepIdx = std::numeric_limits<size_t>::max();
	size_t bufIdx = std::numeric_limits<size_t>::max();
};