/*!
 * \file LambertianPass.h
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
#include "Sink.h"
#include "Source.h"
#include "DepthStencilState.h"

class Graphics;

class LambertianPass : public RenderQueuePass
{
public:
	LambertianPass( Graphics& gfx, std::string name )
		:
		RenderQueuePass( std::move( name ) )
	{
		
		RegisterSink( DirectBufferSink<RenderTarget>::Make( "renderTarget", renderTarget ) );
		RegisterSink( DirectBufferSink<DepthStencilView>::Make( "depthStencil", depthStencil ) );
		RegisterSource( DirectBufferSource<RenderTarget>::Make( "renderTarget", renderTarget ) );
		RegisterSource( DirectBufferSource<DepthStencilView>::Make( "depthStencil", depthStencil ) );
		AddBind( DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::Off ) );
	}
};