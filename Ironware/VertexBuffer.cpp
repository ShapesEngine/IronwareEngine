/*!
 * \class VertexBuffer
 *
 * \brief A vertex buffer class that controls buffers(binds, etc)
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 */
#include "VertexBuffer.h"

void VertexBuffer::Bind( Graphics& gfx ) noexcept
{
	const UINT offset = 0u;
	GetContext( gfx )->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset );
}
