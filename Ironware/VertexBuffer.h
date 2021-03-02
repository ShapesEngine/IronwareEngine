/*!
 * \file VertexBuffer.h
 * \date 2020/09/23 6:23
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A vertex buffer class that controls buffers(binds, etc)
 *
 * TODO:
 *
 * \note Constructor of this class is templated because we would like to have buffers 
 * * of several types.
*/
#pragma once

#include "Bindable.h"
#include "GraphicsExceptionMacros.h"

class VertexBuffer : public Bindable
{
public:
	template<class V>
	VertexBuffer( Graphics& gfx, const std::vector<V>& vertices ) :
		stride( sizeof( V ) )
	{
		INFOMAN( gfx );

		D3D11_BUFFER_DESC descVertexBuffer = {};
		descVertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		descVertexBuffer.Usage = D3D11_USAGE_DEFAULT;
		descVertexBuffer.CPUAccessFlags = 0u;
		descVertexBuffer.MiscFlags = 0u;
		descVertexBuffer.ByteWidth = UINT( sizeof( V ) * vertices.size() );
		descVertexBuffer.StructureByteStride = sizeof( V );
		D3D11_SUBRESOURCE_DATA subresVertexData = {};
		subresVertexData.pSysMem = vertices.data();
		GFX_CALL_THROW_INFO( GetDevice( gfx )->CreateBuffer( &descVertexBuffer, &subresVertexData, &pVertexBuffer ) );
	}

	void Bind( Graphics& gfx ) noexcept override;

protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};