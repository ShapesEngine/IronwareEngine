/*!
 * \file VertexShader.h
 * \date 2020/11/23 6:08
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

class VertexShader : public Bindable
{
public:
	VertexShader( Graphics& gfx, const std::wstring& path );

	inline void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->VSSetShader( pVertexShader.Get(), nullptr, 0u ); }
	inline ID3DBlob* GetBytecode() const noexcept { return pBytecodeBlob.Get(); }

protected:
	Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};

