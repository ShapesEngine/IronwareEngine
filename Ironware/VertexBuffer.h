/*!
 * \file VertexBuffer.h
 * \date 2020/09/23 6:23
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A header that contains a (bindable) vertexbuffer class
 *
*/
#pragma once

#include "Bindable.h"
#include "BindableCollection.h"
#include "IronUtils.h"
#include "Vertex.h"

/*!
 * \class VertexBuffer
 *
 * \ingroup Bindables
 *
 * \brief A vertex buffer class that controls vertex input buffers(binds, etc)
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class VertexBuffer : public Bindable
{
public:
	VertexBuffer( Graphics& gfx, const VertexByteBuffer& vbuff, UINT offset = 0u );
	VertexBuffer( Graphics& gfx, const std::wstring& tag, const VertexByteBuffer& vbuff, UINT offset = 0u );

	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset ); }

	static std::shared_ptr<VertexBuffer> Resolve( Graphics& gfx, const std::wstring& tag, const VertexByteBuffer& vbuff, UINT offset = 0u );
	std::wstring GetUID() const noexcept override { return GenerateUID( tag ); }

	template<TPACK Ignore>
	static std::wstring GenerateUID( const std::wstring& tag, Ignore&&... ignore ) { return GenerateUID_( tag ); }

private:
	static std::wstring GenerateUID_( const std::wstring& tag ) noexcept { return GET_CLASS_WNAME( VertexBuffer ) + L"#" + tag; }

private:
	const std::wstring tag;
	const UINT stride;
	const UINT offset;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};