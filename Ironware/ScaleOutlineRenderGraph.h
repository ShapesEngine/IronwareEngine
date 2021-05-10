/*!
 * \file ScaleOutlineRenderGraph.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "RenderGraph.h"

class Graphics;

class ScaleOutlineRenderGraph : public RenderGraph
{
public:
	ScaleOutlineRenderGraph( Graphics& gfx );
};