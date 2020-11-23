/*!
 * \file PixelShader.h
 * \date 2020/11/23 5:38
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

class PixelShader : public Bindable
{
public:
	PixelShader( Graphics& gfx, const std::wstring& path );

	inline void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetShader( pPixelShader.Get(), nullptr, 0u ); }

protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};

