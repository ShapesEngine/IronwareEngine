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
		descDepthStencil.DepthEnable = FALSE;
		descDepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		descDepthStencil.StencilEnable = TRUE;
		descDepthStencil.StencilWriteMask = 0xFFu;
		descDepthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDepthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	}
	else if( mode == StencilMode::Mask )
	{
		descDepthStencil.DepthEnable = FALSE;
		descDepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		descDepthStencil.StencilEnable = TRUE;
		descDepthStencil.StencilReadMask = 0xFFu;
		descDepthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		descDepthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	}
	else if( mode == StencilMode::DepthOff )
	{
		descDepthStencil.DepthEnable = FALSE;
	}
	else if(mode == StencilMode::DepthReversed)
	{
		descDepthStencil.DepthFunc = D3D11_COMPARISON_GREATER;
	}

	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateDepthStencilState( &descDepthStencil, &pDSState ) );
}

std::wstring DepthStencilState::GenerateUID( StencilMode mode )
{
	std::wstring modeStr;
	switch( mode )
	{
	case DepthStencilState::StencilMode::Off:
		modeStr = L"OFF";
		break;
	case DepthStencilState::StencilMode::Write:
		modeStr = L"WRT";
		break;
	case DepthStencilState::StencilMode::Mask:
		modeStr = L"MSK";
		break;
	case DepthStencilState::StencilMode::DepthOff:
		modeStr = L"DOF";
		break;
	case DepthStencilState::StencilMode::DepthReversed:
		modeStr = L"DRV";
		break;
	};
	return GET_CLASS_WNAME( DepthStencilState ) + L"#" + modeStr;
}
