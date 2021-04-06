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
#include "GraphicsExceptionMacros.h"
#include "Vertex.h"

namespace PipelineBindable
{
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
		template<class V>
		VertexBuffer( Graphics& gfx, const std::vector<V>& vertices, UINT offset = 0 ) :
			stride( sizeof( V ) ),
			offset( offset )
		{
			INFOMAN( gfx );

			D3D11_BUFFER_DESC descVertexBuffer = {};
			descVertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			descVertexBuffer.Usage = D3D11_USAGE_DEFAULT;
			descVertexBuffer.CPUAccessFlags = 0u;
			descVertexBuffer.MiscFlags = 0u;
			descVertexBuffer.ByteWidth = UINT( sizeof( V ) * vertices.size() );
			descVertexBuffer.StructureByteStride = stride;
			D3D11_SUBRESOURCE_DATA subresVertexData = {};
			subresVertexData.pSysMem = vertices.data();
			GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateBuffer( &descVertexBuffer, &subresVertexData, &pVertexBuffer ) );
		}

		VertexBuffer( Graphics& gfx, const VertexByteBuffer& vbuff, UINT offset = 0 );

		void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset ); }

	private:
		const UINT stride;
		const UINT offset;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	};
}