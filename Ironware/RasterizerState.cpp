/*!
 * \file RasterizerState.cpp
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#include "RasterizerState.h"

RasterizerState::RasterizerState( Graphics& gfx, bool isTwoSided ) :
	isTwoSided( isTwoSided )
{
	D3D11_RASTERIZER_DESC descRasterizer = CD3D11_RASTERIZER_DESC{ CD3D11_DEFAULT{} };

	descRasterizer.CullMode = isTwoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;

	GetDevice( gfx )->CreateRasterizerState( &descRasterizer, &pRasterizerState );
}
