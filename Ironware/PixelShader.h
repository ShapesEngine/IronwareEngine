/*!
 * \file PixelShader.h
 * \date 2020/09/23 5:38
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A header that contains a (bindable) pixelshader wrapper class
 *
 * \note It stores created pixel shader data
*/
#pragma once

#include "Bindable.h"

/*!
 * \class PixelShader
 *
 * \ingroup Bindables
 *
 * \brief A PixelShader wrapper class that will be bound in process of graphics pipeline.
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class PixelShader : public Bindable
{
public:
	PixelShader( Graphics& gfx, const std::wstring& path );

	__forceinline void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetShader( pPixelShader.Get(), nullptr, 0u ); }

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};