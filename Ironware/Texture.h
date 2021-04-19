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
#include "BindableCollection.h"

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
	Texture( Graphics& gfx, const std::wstring& path, UINT slot = 0u );
	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetShaderResources( slot, 1u, pTextureView.GetAddressOf() ); }
	static std::shared_ptr<Texture> Resolve( Graphics& gfx, std::wstring& path, UINT slot = 0u ) { return BindableCollection::Resolve<Texture>( gfx, path, slot ); }
	static std::wstring GenerateUID( const std::wstring& path, UINT slot = 0u ) { return GET_CLASS_WNAME( Texture ) + L"#" + path + L"#" + std::to_wstring( slot ); }
	std::wstring GetUID() const noexcept override { return GenerateUID( path, slot ); }

protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	const std::wstring& path;
	const UINT slot;
};