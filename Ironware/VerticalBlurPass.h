/*!
 * \file VerticalBlurPass.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "FullscreenPass.h"
#include "ConstantBuffersEx.h"

class Graphics;
class PixelShader;
class RenderTarget;

class VerticalBlurPass : public FullscreenPass
{
public:
	VerticalBlurPass( std::string name, Graphics& gfx );
	void Execute( Graphics& gfx ) const IFNOEXCEPT override;
private:
	std::shared_ptr<CachingPixelConstantBufferEx> direction;
};