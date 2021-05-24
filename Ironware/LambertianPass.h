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
#include "DepthStencilView.h"
#include "ShadowCameraCBuffer.h"
#include "ShadowSampler.h"

class Graphics;

class LambertianPass : public RenderQueuePass
{
public:
	LambertianPass( Graphics& gfx, std::string name ) :
		RenderQueuePass( std::move( name ) ),
		pShadowCBuf{ std::make_shared<ShadowCameraCBuffer>( gfx ) },
		pShadowSampler{ std::make_shared<ShadowSampler>( gfx ) }
	{
		AddBind( pShadowCBuf );
		AddBind( pShadowSampler );
		RegisterSink( DirectBufferSink<RenderTarget>::Make( "renderTarget", renderTarget ) );
		RegisterSink( DirectBufferSink<DepthStencilView>::Make( "depthStencil", depthStencil ) );
		AddBindSink<Bindable>( "shadowMap" );
		AddBindSink<Bindable>( "shadowControl" );
		RegisterSource( DirectBufferSource<RenderTarget>::Make( "renderTarget", renderTarget ) );
		RegisterSource( DirectBufferSource<DepthStencilView>::Make( "depthStencil", depthStencil ) );
		AddBind( DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::Off ) );
	}

	void BindMainCamera( const Camera& cam ) noexcept
	{
		pMainCamera = &cam;
	}

	void BindShadowCamera( const Camera& cam ) noexcept
	{
		pShadowCBuf->SetCamera( &cam );
	}

	void Execute( Graphics& gfx ) const IFNOEXCEPT override
	{
		assert( pMainCamera );
		pShadowCBuf->Update( gfx );
		pMainCamera->BindToGraphics( gfx );
		RenderQueuePass::Execute( gfx );
	}

private:
	std::shared_ptr<ShadowSampler> pShadowSampler;
	std::shared_ptr<ShadowCameraCBuffer> pShadowCBuf;
	const Camera* pMainCamera = nullptr;
};