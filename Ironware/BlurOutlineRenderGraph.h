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
	void RenderWidgets( Graphics& gfx );

private:
	void SetKernelGauss( int radius, float sigma ) IFNOEXCEPT;
	void SetKernelBox( int radius ) IFNOEXCEPT;
	enum class KernelType
	{
		Gauss,
		Box,
	} kernelType = KernelType::Gauss;
	static constexpr int maxRadius = 7;
	int radius = 4;
	float sigma = 2.f;
	std::shared_ptr<CachingPixelConstantBufferEx> blurKernel;
	std::shared_ptr<CachingPixelConstantBufferEx> blurDirection;
};
