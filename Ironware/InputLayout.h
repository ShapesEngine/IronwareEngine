/*!
 * \file InputLayout.h
 * \date 2020/09/23 5:36
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief An InputLayout wrapper class that will be needed and bound in process of graphics pipeline
 * * and store vertex indices in it.
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
				 const std::vector<D3D11_INPUT_ELEMENT_DESC>& layouts,
				 ID3DBlob* pVertexShaderBytecode );

	__forceinline void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->IASetInputLayout( pInputLayout.Get() ); }

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};

