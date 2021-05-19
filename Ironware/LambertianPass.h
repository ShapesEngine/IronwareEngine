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
#include "Camera.h"

class Graphics;

class LambertianPass : public RenderQueuePass
{
public:
	void BindMainCamera( const Camera& cam ) noexcept
	{
		pMainCamera = &cam;
	}

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

	void Execute( Graphics& gfx ) const IFNOEXCEPT override
	{
		assert( pMainCamera );
		pMainCamera->BindToGraphics( gfx );
		RenderQueuePass::Execute( gfx );
	}

private:
	const Camera* pMainCamera = nullptr;
};