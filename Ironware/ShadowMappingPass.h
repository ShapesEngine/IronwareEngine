/*!
 * \file ShadowMappingPass.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "RenderQueuePass.h"
#include "Job.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "Source.h"
#include "SurfaceEx.h"
#include "RenderTarget.h"
#include "BlendState.h"
#include "NullPixelShader.h"
#include "Camera.h"

#include <vector>

class Graphics;

class ShadowMappingPass : public RenderQueuePass
{
public:
	void BindShadowCamera( const Camera& cam ) noexcept
	{
		pShadowCamera = &cam;
	}

	ShadowMappingPass( Graphics& gfx, std::string name ) :
		RenderQueuePass( std::move( name ) )
	{
		depthStencil = std::make_unique<ShaderInputDepthStencil>( gfx, 3, DepthStencilView::Usage::ShadowDepth );
		AddBind( VertexShader::Resolve( gfx, L"Solid_VS.cso" ) );
		AddBind( NullPixelShader::Resolve( gfx ) );
		AddBind( DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::Off ) );
		AddBind( BlendState::Resolve( gfx, false ) );
		RegisterSource( DirectBindableSource<DepthStencilView>::Make( "map", depthStencil ) );
	}

	void Execute( Graphics& gfx ) const IFNOEXCEPT override
	{
		depthStencil->Clear( gfx );
		pShadowCamera->BindToGraphics( gfx );
		RenderQueuePass::Execute( gfx );
	}

	void DumpShadowMap( Graphics& gfx, const std::wstring& path ) const
	{
		depthStencil->ToSurface( gfx ).Save( path );
	}

private:
	const Camera* pShadowCamera = nullptr;
};