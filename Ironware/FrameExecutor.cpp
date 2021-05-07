/*!
 * \file FrameExecutor.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "FrameExecutor.h"
#include "BindableCommon.h"
#include "DepthStencilView.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "BlendState.h"

FrameExecutor::FrameExecutor( Graphics & gfx ) :
	dsv( gfx, gfx.GetWidth(), gfx.GetHeight() ),
	rt1( gfx, gfx.GetWidth(), gfx.GetHeight() ),
	rt2( gfx, gfx.GetWidth(), gfx.GetHeight() ),
	blur( gfx )
{
	namespace dx = DirectX;
	VertexLayout lay;
	lay.Append( VertexLayout::ElementType::Position2D );
	VertexByteBuffer vbb{ lay };

	vbb.EmplaceBack( dx::XMFLOAT2{ -1.f, 1.f } );
	vbb.EmplaceBack( dx::XMFLOAT2{ 1.f, 1.f } );
	vbb.EmplaceBack( dx::XMFLOAT2{ -1.f, -1.f } );
	vbb.EmplaceBack( dx::XMFLOAT2{ 1.f, -1.f } );

	pVbFull = VertexBuffer::Resolve( gfx, L"%Full", std::move( vbb ) );
	pIbFull = IndexBuffer::Resolve( gfx, L"%Full", { 0, 1, 2, 2, 1, 3 } );
	pVsFull = VertexShader::Resolve( gfx, L"Fullscreen_VS.cso" );
	pLayoutFull = InputLayout::Resolve( gfx, lay, pVsFull->GetBytecode() );
	pSamplerFull = Sampler::Resolve( gfx, false, true );
}

void FrameExecutor::Execute( Graphics& gfx ) IFNOEXCEPT
{
	dsv.Clear( gfx );
	rt1.Clear( gfx );
	rt1.BindAsTarget( gfx, dsv );

	BlendState::Resolve( gfx, false )->Bind( gfx );
	DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::Off )->Bind( gfx );
	rqs[0].Execute( gfx );

	rt2.BindAsTarget( gfx );
	rt1.BindAsTexture( gfx, 0u );
	pVbFull->Bind( gfx );
	pIbFull->Bind( gfx );
	pVsFull->Bind( gfx );
	pLayoutFull->Bind( gfx );
	pSamplerFull->Bind( gfx );
	blur.Bind( gfx );
	blur.SetHorizontal( gfx );
	gfx.DrawIndexed( pIbFull->GetCount() );

	gfx.BindSwapBuffer();
	rt2.BindAsTexture( gfx, 0u );
	blur.SetVertical( gfx );
	gfx.DrawIndexed( pIbFull->GetCount() );
}

void FrameExecutor::Reset() noexcept
{
	for( auto& q : rqs )
	{
		q.Reset();
	}
}