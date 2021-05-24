/*!
 * \file BlurOutlineRenderGraph.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "BlurOutlineRenderGraph.h"
#include "BufferClearPass.h"
#include "LambertianPass.h"
#include "OutlineDrawingPass.h"
#include "OutlineMaskGenerationPass.h"
#include "Source.h"
#include "HorizontalBlurPass.h"
#include "VerticalBlurPass.h"
#include "BlurOutlineDrawingPass.h"
#include "WireframePass.h"
#include "RenderTarget.h"
#include "DynamicConstantBuffer.h"
#include "ShadowMappingPass.h"
#include "IronUtils.h"
#include "IronMath.h"
#include "imgui/imgui.h"

BlurOutlineRenderGraph::BlurOutlineRenderGraph( Graphics& gfx ) :
	RenderGraph( gfx )
{
	{
		auto pass = std::make_unique<BufferClearPass>( "clearRT" );
		pass->SetSinkLinkage( "buffer", "$.backbuffer" );
		AppendPass( std::move( pass ) );
	}
	{
		auto pass = std::make_unique<BufferClearPass>( "clearDS" );
		pass->SetSinkLinkage( "buffer", "$.masterDepth" );
		AppendPass( std::move( pass ) );
	}
	{
		auto pass = std::make_unique<ShadowMappingPass>( gfx, "shadowMap" );
		AppendPass( std::move( pass ) );
	}

	// setup shadow control buffer
	{
		{
			RawLayout l;
			l.Add<Integer>( "pcfLevel" );
			l.Add<Float>( "depthBias" );
			Buffer buf{ std::move( l ) };
			buf["pcfLevel"] = 0;
			buf["depthBias"] = 0.0005f;
			shadowControl = std::make_shared<CachingPixelConstantBufferEx>( gfx, buf, 2 );
			AddGlobalSource( DirectBindableSource<CachingPixelConstantBufferEx>::Make( "shadowControl", shadowControl ) );
		}
	}

	{
		auto pass = std::make_unique<LambertianPass>( gfx, "lambertian" );
		pass->SetSinkLinkage( "shadowMap", "shadowMap.map" );
		pass->SetSinkLinkage( "renderTarget", "clearRT.buffer" );
		pass->SetSinkLinkage( "depthStencil", "clearDS.buffer" );
		pass->SetSinkLinkage( "shadowControl", "$.shadowControl" );
		AppendPass( std::move( pass ) );
	}
	{
		auto pass = std::make_unique<OutlineMaskGenerationPass>( gfx, "outlineMask" );
		pass->SetSinkLinkage( "depthStencil", "lambertian.depthStencil" );
		AppendPass( std::move( pass ) );
	}

	// setup blur constant buffers
	{
		{
			RawLayout l;
			l.Add<Integer>( "nTaps" );
			l.Add<Array>( "coefficients" );
			l["coefficients"].Set<Float>( maxRadius * 2 + 1 );
			Buffer buf{ std::move( l ) };
			blurKernel = std::make_shared<CachingPixelConstantBufferEx>( gfx, buf, 0 );
			SetKernelGauss( radius, sigma );
			AddGlobalSource( DirectBindableSource<CachingPixelConstantBufferEx>::Make( "blurKernel", blurKernel ) );
		}
		{
			RawLayout l;
			l.Add<Bool>( "isHorizontal" );
			Buffer buf{ std::move( l ) };
			blurDirection = std::make_shared<CachingPixelConstantBufferEx>( gfx, buf, 1 );
			AddGlobalSource( DirectBindableSource<CachingPixelConstantBufferEx>::Make( "blurDirection", blurDirection ) );
		}
	}

	{
		auto pass = std::make_unique<BlurOutlineDrawingPass>( gfx, "outlineDraw", gfx.GetWidth(), gfx.GetHeight() );
		AppendPass( std::move( pass ) );
	}
	{
		auto pass = std::make_unique<HorizontalBlurPass>( "horizontal", gfx, gfx.GetWidth(), gfx.GetHeight() );
		pass->SetSinkLinkage( "scratchIn", "outlineDraw.scratchOut" );
		pass->SetSinkLinkage( "kernel", "$.blurKernel" );
		pass->SetSinkLinkage( "direction", "$.blurDirection" );
		AppendPass( std::move( pass ) );
	}
	{
		auto pass = std::make_unique<VerticalBlurPass>( "vertical", gfx );
		pass->SetSinkLinkage( "renderTarget", "lambertian.renderTarget" );
		pass->SetSinkLinkage( "depthStencil", "outlineMask.depthStencil" );
		pass->SetSinkLinkage( "scratchIn", "horizontal.scratchOut" );
		pass->SetSinkLinkage( "kernel", "$.blurKernel" );
		pass->SetSinkLinkage( "direction", "$.blurDirection" );
		AppendPass( std::move( pass ) );
	}
	{
		auto pass = std::make_unique<WireframePass>( gfx, "wireframe" );
		pass->SetSinkLinkage( "renderTarget", "vertical.renderTarget" );
		pass->SetSinkLinkage( "depthStencil", "vertical.depthStencil" );
		AppendPass( std::move( pass ) );
	}
	SetSinkTarget( "backbuffer", "wireframe.renderTarget" );

	Finalize();
}

void BlurOutlineRenderGraph::SetKernelGauss( int radius, float sigma ) IFNOEXCEPT
{
	assert( radius <= maxRadius );
	auto k = blurKernel->GetBuffer();
	const int nTaps = radius * 2 + 1;
	k["nTaps"] = nTaps;
	float sum = 0.f;
	for( int i = 0; i < nTaps; i++ )
	{
		const auto x = float( i - radius );
		const auto g = gauss( x, sigma );
		sum += g;
		k["coefficients"][i] = g;
	}
	for( int i = 0; i < nTaps; i++ )
	{
		k["coefficients"][i] = (float)k["coefficients"][i] / sum;
	}
	blurKernel->SetBuffer( k );
}

void BlurOutlineRenderGraph::SetKernelBox( int radius ) IFNOEXCEPT
{
	assert( radius <= maxRadius );
	auto k = blurKernel->GetBuffer();
	const int nTaps = radius * 2 + 1;
	k["nTaps"] = nTaps;
	const float c = 1.f / nTaps;
	for( int i = 0; i < nTaps; i++ )
	{
		k["coefficients"][i] = c;
	}
	blurKernel->SetBuffer( k );
}

void BlurOutlineRenderGraph::RenderWindows( Graphics& gfx )
{
	RenderShadowWindow( gfx );
	RenderKernelWindow( gfx );
}

void BlurOutlineRenderGraph::DumpShadowMap( Graphics & gfx, const std::wstring & path )
{
	dynamic_cast<ShadowMappingPass&>( FindPassByName( "shadowMap" ) ).DumpShadowMap( gfx, path );
}

void BlurOutlineRenderGraph::BindMainCamera( Camera & cam )
{
	dynamic_cast<LambertianPass&>( FindPassByName( "lambertian" ) ).BindMainCamera( cam );
}

void BlurOutlineRenderGraph::BindShadowCamera( Camera & cam )
{
	dynamic_cast<ShadowMappingPass&>( FindPassByName( "shadowMap" ) ).BindShadowCamera( cam );
	dynamic_cast<LambertianPass&>( FindPassByName( "lambertian" ) ).BindShadowCamera( cam );
}

void BlurOutlineRenderGraph::RenderKernelWindow( Graphics & gfx )
{
	if( ImGui::Begin( "Kernel" ) )
	{
		bool filterChanged = false;
		{
			const char* items[] = { "Gauss","Box" };
			static const char* curItem = items[0];
			if( ImGui::BeginCombo( "Filter Type", curItem ) )
			{
				for( auto & item : items )
				{
					const bool isSelected = ( curItem == item );
					if( ImGui::Selectable( item, isSelected ) )
					{
						filterChanged = true;
						curItem = item;
						if( curItem == items[0] )
						{
							kernelType = KernelType::Gauss;
						}
						else if( curItem == items[1] )
						{
							kernelType = KernelType::Box;
						}
					}
					if( isSelected )
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}

		bool radChange = ImGui::SliderInt( "Radius", &radius, 0, maxRadius );
		bool sigChange = ImGui::SliderFloat( "Sigma", &sigma, 0.1f, 10.f );
		if( radChange || sigChange || filterChanged )
		{
			if( kernelType == KernelType::Gauss )
			{
				SetKernelGauss( radius, sigma );
			}
			else if( kernelType == KernelType::Box )
			{
				SetKernelBox( radius );
			}
		}
	}
	ImGui::End();
}

void BlurOutlineRenderGraph::RenderShadowWindow( Graphics & gfx )
{
	if( ImGui::Begin( "Shadows" ) )
	{
		auto ctrl = shadowControl->GetBuffer();
		bool pfcChange = ImGui::SliderInt( "PCF Level", &ctrl["pcfLevel"], 0, 4 );
		bool biasChange = ImGui::SliderFloat( "Depth Bias", &ctrl["depthBias"], 0.0f, 0.1f, "%.6f", 3.6f );
		if( pfcChange || biasChange )
		{
			shadowControl->SetBuffer( ctrl );
		}
	}
	ImGui::End();
}
