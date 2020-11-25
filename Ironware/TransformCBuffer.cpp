/*!
 * \class TransformCBuffer
 *
 * \brief A Transform Constant Buffer wrapper child class that will be bound in process of graphics pipeline.
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
#include "TransformCBuffer.h"

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCBuffer::pVertConstBuffer;

TransformCBuffer::TransformCBuffer( Graphics& gfx, const Drawable& parent ) :
	parent( parent )
{
	if( !pVertConstBuffer )
	{
		pVertConstBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>( gfx );
	}
}

inline void TransformCBuffer::Bind( Graphics & gfx ) noexcept
{
	pVertConstBuffer->Update( gfx, DirectX::XMMatrixTranspose( parent.GetTransformXM() * gfx.GetProjection() ) );
	pVertConstBuffer->Bind( gfx );
}
