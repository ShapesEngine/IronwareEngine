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
#include <d3dcompiler.h>

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

	__forceinline void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->VSSetShader( pVertexShader.Get(), nullptr, 0u ); }
	__forceinline ID3DBlob* GetBytecode() const noexcept { return pBytecodeBlob.Get(); }

protected:
	Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};

