/*!
 * \file BlendState.cpp
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#include "BlendState.h"
#include "GraphicsExceptionMacros.h"

BlendState::BlendState( Graphics& gfx, bool isBlending ) :
	isBlending( isBlending )
{
	INFOMAN( gfx );

	D3D11_BLEND_DESC descBlend = {};
	auto& rt = descBlend.RenderTarget[0];

	if( isBlending )
	{
		rt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		rt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		rt.BlendOp = D3D11_BLEND_OP_ADD;
		rt.SrcBlendAlpha = D3D11_BLEND_ZERO;
		rt.DestBlendAlpha = D3D11_BLEND_ZERO;
		rt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	}

	rt.BlendEnable = isBlending ? TRUE : FALSE;
	rt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateBlendState( &descBlend, &pBlendState ) );
}