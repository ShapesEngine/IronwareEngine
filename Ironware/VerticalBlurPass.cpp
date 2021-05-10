/*!
 * \file VerticalBlurPass.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#include "VerticalBlurPass.h"
#include "Sink.h"
#include "Source.h"
#include "PixelShader.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "Sampler.h"

	VerticalBlurPass::VerticalBlurPass( std::string name, Graphics& gfx ) :
		FullscreenPass( std::move( name ), gfx )
	{
		AddBind( PixelShader::Resolve( gfx, L"BlurOutline_PS.cso" ) );
		AddBind( BlendState::Resolve( gfx, true ) );
		AddBind( DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::Mask ) );
		AddBind( Sampler::Resolve( gfx, Sampler::Type::Bilinear, true ) );

		AddBindSink<RenderTarget>( "scratchIn" );
		AddBindSink<CachingPixelConstantBufferEx>( "kernel" );
		RegisterSink( DirectBindableSink<CachingPixelConstantBufferEx>::Make( "direction", direction ) );
		RegisterSink( DirectBufferSink<RenderTarget>::Make( "renderTarget", renderTarget ) );
		RegisterSink( DirectBufferSink<DepthStencilView>::Make( "depthStencil", depthStencil ) );

		RegisterSource( DirectBufferSource<RenderTarget>::Make( "renderTarget", renderTarget ) );
		RegisterSource( DirectBufferSource<DepthStencilView>::Make( "depthStencil", depthStencil ) );
	}

	// see the note on HorizontalBlurPass::Execute
	void VerticalBlurPass::Execute( Graphics& gfx ) const IFNOEXCEPT
	{
		auto buf = direction->GetBuffer();
		buf["isHorizontal"] = false;
		direction->SetBuffer( buf );

		direction->Bind( gfx );
		FullscreenPass::Execute( gfx );
	}