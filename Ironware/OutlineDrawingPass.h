/*!
 * \file OutlineDrawingPass.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "RenderQueuePass.h"
#include "Job.h"
#include <vector>
#include "PixelShader.h"
#include "VertexShader.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"

class Graphics;

class OutlineDrawingPass : public RenderQueuePass
{
public:
	OutlineDrawingPass( Graphics& gfx, std::string name )
		:
		RenderQueuePass( std::move( name ) )
	{
		RegisterSink( DirectBufferSink<RenderTarget>::Make( "renderTarget", renderTarget ) );
		RegisterSink( DirectBufferSink<DepthStencilView>::Make( "depthStencil", depthStencil ) );
		RegisterSource( DirectBufferSource<RenderTarget>::Make( "renderTarget", renderTarget ) );
		RegisterSource( DirectBufferSource<DepthStencilView>::Make( "depthStencil", depthStencil ) );
		AddBind( VertexShader::Resolve( gfx, L"Solid_VS.cso" ) );
		AddBind( PixelShader::Resolve( gfx, L"Solid_PS.cso" ) );
		AddBind( DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::Mask ) );
		AddBind( RasterizerState::Resolve( gfx, false ) );
	}
};