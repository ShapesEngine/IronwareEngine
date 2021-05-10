/*!
 * \file HorizontalBlurPass.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "HorizontalBlurPass.h"
#include "PixelShader.h"
#include "RenderTarget.h"
#include "Sink.h"
#include "Source.h"
#include "BlendState.h"
#include "Sampler.h"
#include "ConstantBuffersEx.h"

HorizontalBlurPass::HorizontalBlurPass( std::string name, Graphics& gfx, uint32_t fullWidth, uint32_t fullHeight )
	:
	FullscreenPass( std::move( name ), gfx )
{
	AddBind( PixelShader::Resolve( gfx, L"BlurOutline_PS.cso" ) );
	AddBind( BlendState::Resolve( gfx, false ) );
	AddBind( Sampler::Resolve( gfx, Sampler::Type::Point, true ) );

	AddBindSink<RenderTarget>( "scratchIn" );
	AddBindSink<CachingPixelConstantBufferEx>( "kernel" );
	RegisterSink( DirectBindableSink<CachingPixelConstantBufferEx>::Make( "direction", direction ) );

	// the renderTarget is internally sourced and then exported as a Bindable
	renderTarget = std::make_shared<ShaderInputRenderTarget>( gfx, fullWidth / 2, fullHeight / 2, 0u );
	RegisterSource( DirectBindableSource<RenderTarget>::Make( "scratchOut", renderTarget ) );
}

// this override is necessary because we cannot (yet) link input bindables directly into
// the container of bindables (mainly because vector growth buggers references)
void HorizontalBlurPass::Execute( Graphics& gfx ) const IFNOEXCEPT
{
	auto buf = direction->GetBuffer();
	buf["isHorizontal"] = true;
	direction->SetBuffer( buf );

	direction->Bind( gfx );
	FullscreenPass::Execute( gfx );
}