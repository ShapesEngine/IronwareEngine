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

BlendState::BlendState( Graphics & gfx, bool isBlending, std::optional<float> factor ) :
	isBlending( isBlending )
{
	INFOMAN( gfx );

	D3D11_BLEND_DESC descBlend = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
	auto& rt = descBlend.RenderTarget[0];

	if( isBlending )
	{
		rt.BlendEnable = TRUE;
		if( factor )
		{
			blendFactors.emplace();
			blendFactors->fill( *factor );
			rt.SrcBlend = D3D11_BLEND_BLEND_FACTOR;
			rt.DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;
		}
		else
		{
			rt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			rt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		}
	}

	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateBlendState( &descBlend, &pBlendState ) );
}

void BlendState::Bind( Graphics & gfx ) noexcept
{
	const FLOAT* pBlendFactors = blendFactors ? blendFactors->data() : nullptr;
	GetContext( gfx )->OMSetBlendState( pBlendState.Get(), pBlendFactors, 0xffffffffu );
}

void BlendState::SetFactor( float factor ) IFNOEXCEPT
{
	assert( blendFactors );
	blendFactors->fill( factor );
}

float BlendState::GetFactor() const IFNOEXCEPT
{
	assert( blendFactors );
	return blendFactors->front();
}

std::wstring BlendState::GenerateUID( bool isBlending, std::optional<float> factor )
{
	const auto blendingStr = ( isBlending ? L"bt" : L"bf" );
	const auto factorStr = ( factor ? std::to_wstring( *factor ) : L"?" );
	return GET_CLASS_WNAME( BlendState ) + L"#" + blendingStr + L"#f" + factorStr;
}
