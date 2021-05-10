/*!
 * \file HorizontalBlurPass.h
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

class HorizontalBlurPass : public FullscreenPass
{
public:
	HorizontalBlurPass( std::string name, Graphics& gfx, uint32_t fullWidth, uint32_t fullHeight );
	void Execute( Graphics& gfx ) const IFNOEXCEPT override;

private:
	std::shared_ptr<CachingPixelConstantBufferEx> direction;
};