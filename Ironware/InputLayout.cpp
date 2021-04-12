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

InputLayout::InputLayout( Graphics& gfx, const VertexLayout& layout_in, ID3DBlob* pVertexShaderBytecode ) :
	layout( layout_in )
{
	INFOMAN( gfx );

	const auto d3dlayout = layout.GetD3DLayout();

	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateInputLayout(
		d3dlayout.data(), (UINT)d3dlayout.size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout
	) );
}
