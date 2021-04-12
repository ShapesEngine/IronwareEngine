/*!
 * \file VertexShader.h
 * \date 2020/09/23 6:08
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A header that contains a (bindable) vertexshader class
 *
 * \note It stores: bytecode, as its bufferpointer and buffer size are needed in creation process
 * *				vertexShader is created vertexshader data
*/
#pragma once

#include "Bindable.h"
#include "BindableCollection.h"
#include "IronUtils.h"

#include <d3dcompiler.h>

#include <typeinfo>

/*!
 * \class VertexShader
 *
 * \ingroup Bindables
 *
 * \brief A VertexShader wrapper class that will be bound in process of graphics pipeline.
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class VertexShader : public Bindable
{
public:
	VertexShader( Graphics& gfx, const std::wstring& path );

	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->VSSetShader( pVertexShader.Get(), nullptr, 0u ); }
	ID3DBlob* GetBytecode() const noexcept { return pBytecodeBlob.Get(); }
	static std::shared_ptr<Bindable> Resolve( Graphics& gfx, const std::wstring& path ) noexcept { return BindableCollection::Resolve<VertexShader>( gfx, path ); }
	static std::wstring GenerateUID( const std::wstring path ) noexcept { return to_wide( typeid( VertexShader ).name() ) + L"#" + path; }
	std::wstring GetUID() const noexcept override { return GenerateUID( path ); }

protected:
	std::wstring path;
	Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};