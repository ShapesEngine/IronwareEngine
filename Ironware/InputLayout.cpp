/*!
 * \class InputLayout
 *
 * \brief 
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
#include "InputLayout.h"
#include "GraphicsExceptionMacros.h"

InputLayout::InputLayout( Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode )
{
	INFOMAN( gfx );

	GFX_THROW_INFO( GetDevice( gfx )->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout
	) );
}
