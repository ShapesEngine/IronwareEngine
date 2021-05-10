/*!
 * \file BlurOutlineDrawingPass.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "RenderQueuePass.h"
#include "Pass.h"
#include "Job.h"
#include <vector>
#include "PixelShader.h"
#include "VertexShader.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "Source.h"
#include "RenderTarget.h"
#include "BlendState.h"

class Graphics;

class BlurOutlineDrawingPass : public RenderQueuePass
{
public:
	BlurOutlineDrawingPass( Graphics& gfx, std::string name, uint32_t fullWidth, uint32_t fullHeight )
		:
		RenderQueuePass( std::move( name ) )
	{
		renderTarget = std::make_unique<ShaderInputRenderTarget>( gfx, fullWidth / 2, fullHeight / 2, 0 );
		AddBind( VertexShader::Resolve( gfx, L"Solid_VS.cso" ) );
		AddBind( PixelShader::Resolve( gfx, L"Solid_PS.cso" ) );
		AddBind( DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::Mask ) );
		AddBind( BlendState::Resolve( gfx, false ) );
		RegisterSource( DirectBindableSource<RenderTarget>::Make( "scratchOut", renderTarget ) );
	}
	void Execute( Graphics& gfx ) const IFNOEXCEPT override
	{
		renderTarget->Clear( gfx );
		RenderQueuePass::Execute( gfx );
	}
};