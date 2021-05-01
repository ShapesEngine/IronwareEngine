/*!
 * \file NullPixelShader.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "Bindable.h"
#include "BindableCollection.h"

 /**
  * @brief Undefined pixel shader that binds null shader
 */
class NullPixelShader : public Bindable
{
public:
	NullPixelShader( Graphics& gfx ) {}
	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->PSSetShader( nullptr, nullptr, 0u ); }
	static std::shared_ptr<NullPixelShader> Resolve( Graphics& gfx ) { return BindableCollection::Resolve<NullPixelShader>( gfx ); }
	static std::wstring GenerateUID() { return GET_CLASS_WNAME( NullPixelShader ); }
	std::wstring GetUID() const noexcept override { return GenerateUID(); }
};