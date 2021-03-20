/*!
 * \file InputLayout.h
 * \date 2020/09/23 5:36
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A header that contains a (bindable) inputlayout class
 *
*/
#pragma once

#include "Bindable.h"

/*!
 * \class InputLayout
 *
 * \ingroup Bindables
 *
 * \brief An InputLayout wrapper class that will be needed and bound in process of graphics pipeline
 * * and store vertex indices in it.
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
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

