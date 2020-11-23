/*!
 * \class VertexBuffer
 *
 * \brief 
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
#include "VertexBuffer.h"

void VertexBuffer::Bind( Graphics& gfx ) noexcept
{
	const UINT offset = 0u;
	GetContext( gfx )->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset );
}
