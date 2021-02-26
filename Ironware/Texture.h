/*!
 * \file Texture.h
 * \date 2020/12/06 11:13
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief This file contains Texture class.
 * * Texture class is responsible for creating & loading the
 * * texture to the GPU
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture( Graphics& gfx, const class Surface& sur );
	__forceinline void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetShaderResources( 0u, 1u, pTextureView.GetAddressOf() ); }

protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};

