/*!
 * \file InputLayout.h
 * \date 2020/11/23 5:36
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief 
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "Bindable.h"

class InputLayout : public Bindable
{
public:
	InputLayout( Graphics& gfx,
				 const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
				 ID3DBlob* pVertexShaderBytecode );

	inline void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->IASetInputLayout( pInputLayout.Get() ); }

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};

