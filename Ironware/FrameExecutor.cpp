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
	rt( gfx, gfx.GetWidth(), gfx.GetHeight() )
{
	namespace dx = DirectX;
	const auto lay = VertexLayout{}.Append( VertexLayout::ElementType::Position2D );
	VertexByteBuffer vbb{ lay };

	vbb.EmplaceBack( dx::XMFLOAT2{ -1.f, 1.f } );
	vbb.EmplaceBack( dx::XMFLOAT2{ 1.f, 1.f } );
	vbb.EmplaceBack( dx::XMFLOAT2{ -1.f, -1.f } );
	vbb.EmplaceBack( dx::XMFLOAT2{ 1.f, -1.f } );

	pVbFull = VertexBuffer::Resolve( gfx, L"%Full", std::move( vbb ) );
	pIbFull = IndexBuffer::Resolve( gfx, L"%Full", { 0, 1, 2, 2, 1, 3 } );
	pVsFull = VertexShader::Resolve( gfx, L"Fullscreen_VS.cso" );
	pPsFull = PixelShader::Resolve( gfx, L"Filter_PS.cso" );
	pLayoutFull = InputLayout::Resolve( gfx, lay, pVsFull->GetBytecode() );
	pSampler = Sampler::Resolve( gfx, false, true );
	pBlender = BlendState::Resolve( gfx, true );
}

void FrameExecutor::Execute( Graphics& gfx ) const IFNOEXCEPT
{
	dsv.Clear( gfx );
	rt.Clear( gfx );
	gfx.BindSwapBuffer( dsv );

	BlendState::Resolve( gfx, false )->Bind( gfx );
	DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::Off )->Bind( gfx );
	rqs[0].Execute( gfx );

	DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::Write )->Bind( gfx );
	NullPixelShader::Resolve( gfx )->Bind( gfx );
	rqs[1].Execute( gfx );

	rt.BindAsTarget( gfx );
	DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::Off )->Bind( gfx );
	rqs[2].Execute( gfx );

	gfx.BindSwapBuffer( dsv );
	rt.BindAsTexture( gfx, 0u );
	pVbFull->Bind( gfx );
	pIbFull->Bind( gfx );
	pVsFull->Bind( gfx );
	pPsFull->Bind( gfx );
	pLayoutFull->Bind( gfx );
	pSampler->Bind( gfx );
	pBlender->Bind( gfx );
	DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::Mask )->Bind( gfx );
	gfx.DrawIndexed( pIbFull->GetCount() );
}

void FrameExecutor::Reset() noexcept
{
	for( auto& q : rqs )
	{
		q.Reset();
	}
}