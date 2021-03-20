/*!
 * \file Texture.h
 * \date 2020/11/06 11:13
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief This file contains a (bindable) Texture class.
 *
*/
#pragma once

#include "Bindable.h"

/*!
 * \class Texture
 *
 * \ingroup Bindables
 *
 * \brief Responsible class for creating & loading the texture to the GPU
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class Texture : public Bindable
{
public:
	Texture( Graphics& gfx, const class Surface& sur, UINT slot = 0u );
	__forceinline void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetShaderResources( slot, 1u, pTextureView.GetAddressOf() ); }

protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	const UINT slot;
};