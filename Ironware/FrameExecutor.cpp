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

void FrameExecutor::Execute( Graphics& gfx ) const noexcept( !IS_DEBUG )
{
	DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::Off )->Bind( gfx );
	rqs[0].Execute( gfx );

	DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::Write )->Bind( gfx );
	NullPixelShader::Resolve( gfx )->Bind( gfx );
	rqs[1].Execute( gfx );

	DepthStencilState::Resolve( gfx, DepthStencilState::StencilMode::Mask )->Bind( gfx );
	struct SolidColorCBuff
	{
		DirectX::XMFLOAT4 color = { 1.f, 1.f, 0.5f, 1.f };
	} colCbuff;
	PixelConstantBuffer<SolidColorCBuff>::Resolve( gfx, colCbuff, 1u )->Bind( gfx );
	rqs[2].Execute( gfx );
}

void FrameExecutor::Reset() noexcept
{
	for( auto& q : rqs )
	{
		q.Reset();
	}
}