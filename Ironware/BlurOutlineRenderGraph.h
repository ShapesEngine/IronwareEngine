/*!
 * \file BlurOutlineRenderGraph.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "RenderGraph.h"
#include "ConstantBuffersEx.h"

#include <memory>

class Graphics;
class Bindable;
class RenderTarget;

class BlurOutlineRenderGraph : public RenderGraph
{
public:
	BlurOutlineRenderGraph( Graphics& gfx );
private:
	// private functions
	void SetKernelGauss( int radius, float sigma ) IFNOEXCEPT;
	// private data
	static constexpr int maxRadius = 7;
	static constexpr int radius = 4;
	static constexpr float sigma = 2.0f;
	std::shared_ptr<CachingPixelConstantBufferEx> blurKernel;
	std::shared_ptr<CachingPixelConstantBufferEx> blurDirection;
};
