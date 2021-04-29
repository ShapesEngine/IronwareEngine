/*!
 * \file DepthStencilState.cpp
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#include "DepthStencilState.h"
#include "GraphicsExceptionMacros.h"

#include <cassert>

DepthStencilState::DepthStencilState( Graphics & gfx, StencilMode mode ) :
	mode( mode )
{
	INFOMAN( gfx );

	D3D11_DEPTH_STENCIL_DESC descDepthStencil = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

	if( mode == StencilMode::Write )
	{
		descDepthStencil.StencilEnable = TRUE;
		descDepthStencil.StencilWriteMask = 0xFFu;
		descDepthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDepthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	}
	else if( mode == StencilMode::Mask )
	{
		descDepthStencil.DepthEnable = FALSE;
		descDepthStencil.StencilEnable = TRUE;
		descDepthStencil.StencilReadMask = 0xFFu;
		descDepthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		descDepthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	}

	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateDepthStencilState( &descDepthStencil, &pDSState ) );
}

std::wstring DepthStencilState::GetUID() const noexcept
{
	assert( false && "Not yet integrated with bCollection system!" );
	return L"?";
}
