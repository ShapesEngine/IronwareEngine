/*!
 * \file InputLayout.cpp
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 *
 *
 */
#include "InputLayout.h"
#include "GraphicsExceptionMacros.h"

InputLayout::InputLayout( Graphics& gfx, const VertexLayout& layout_in, const VertexShader& vs ) :
	layout( layout_in ),
	pVShader( &vs )
{
	INFOMAN( gfx );

	const auto d3dlayout = layout.GetD3DLayout();
	const auto pBytecode = pVShader->GetBytecode();

	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateInputLayout(
		d3dlayout.data(), (UINT)d3dlayout.size(),
		pBytecode->GetBufferPointer(),
		pBytecode->GetBufferSize(),
		&pInputLayout
	) );
}
