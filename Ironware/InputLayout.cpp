/*!
 * \class InputLayout
 *
 * \brief An InputLayout wrapper class that will be needed and bound in process of graphics pipeline
 * * and store vertex indices in it.
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 */
#include "InputLayout.h"
#include "GraphicsExceptionMacros.h"

InputLayout::InputLayout( Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layouts, ID3DBlob* pVertexShaderBytecode )
{
	INFOMAN( gfx );

	GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateInputLayout(
		layouts.data(), (UINT)layouts.size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout
	) );
}
