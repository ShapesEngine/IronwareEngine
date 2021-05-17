/*!
 * \file WireframePass.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "RenderQueuePass.h"
#include "Job.h"
#include "Sink.h"
#include "Source.h"
#include "DepthStencilState.h"
#include "Graphics.h"

#include <vector>

class WireframePass : public RenderQueuePass
{
public:
	WireframePass( Graphics& gfx, std::string name ) :
		RenderQueuePass( std::move( name ) )
	{
		RegisterSink( DirectBufferSink<RenderTarget>::Make( "renderTarget", renderTarget ) );
		RegisterSink( DirectBufferSink<DepthStencilView>::Make( "depthStencil", depthStencil ) );
		RegisterSource( DirectBufferSource<RenderTarget>::Make( "renderTarget", renderTarget ) );
		RegisterSource( DirectBufferSource<DepthStencilView>::Make( "depthStencil", depthStencil ) );
		AddBind( DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::DepthReversed ) );
	}
};