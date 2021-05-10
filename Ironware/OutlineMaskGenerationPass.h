/*!
 * \file OutlineMaskGenerationPass.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "RenderQueuePass.h"
#include "Job.h"
#include "NullPixelShader.h"
#include "VertexShader.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"

#include <vector>

class Graphics;

class OutlineMaskGenerationPass : public RenderQueuePass
{
public:
	OutlineMaskGenerationPass( Graphics& gfx, std::string name )
		:
		RenderQueuePass( std::move( name ) )
	{
		
		RegisterSink( DirectBufferSink<DepthStencilView>::Make( "depthStencil", depthStencil ) );
		RegisterSource( DirectBufferSource<DepthStencilView>::Make( "depthStencil", depthStencil ) );
		AddBind( VertexShader::Resolve( gfx, L"Solid_VS.cso" ) );
		AddBind( NullPixelShader::Resolve( gfx ) );
		AddBind( DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::Write ) );
		AddBind( RasterizerState::Resolve( gfx, false ) );
	}
};