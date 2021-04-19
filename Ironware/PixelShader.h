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
#include "BindableCollection.h"

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

	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetShader( pPixelShader.Get(), nullptr, 0u ); }
	static std::shared_ptr<PixelShader> Resolve( Graphics& gfx, const std::wstring& path ) { return BindableCollection::Resolve<PixelShader>( gfx, path ); }
	static std::wstring GenerateUID( const std::wstring& path ) { return GET_CLASS_WNAME( PixelShader ) + L"#" + path; }
	std::wstring GetUID() const noexcept override { return GenerateUID( path ); }

protected:
	std::wstring path;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};