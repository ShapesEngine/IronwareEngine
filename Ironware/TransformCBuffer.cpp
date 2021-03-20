/*!
 * \file TransformCBuffer.cpp
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 *
 * 
 */
#include "TransformCBuffer.h"

std::unique_ptr<VertexConstantBuffer<TransformCBuffer::Transforms>> TransformCBuffer::pVertConstBuffer;

TransformCBuffer::TransformCBuffer( Graphics& gfx, const Drawable& parent, UINT slot ) :
	parent( parent )
{
	if( !pVertConstBuffer )
	{
		pVertConstBuffer = std::make_unique<VertexConstantBuffer<Transforms>>( gfx, slot );
	}
}

 void TransformCBuffer::Bind( Graphics & gfx ) noexcept
{
	const auto modelView = parent.GetTransformXM() * gfx.GetCamera();
	const Transforms transforms =
	{
		DirectX::XMMatrixTranspose( modelView ),
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
		)
	};
	// M * V * P => model * view * projection
	pVertConstBuffer->Update( gfx, transforms );
	pVertConstBuffer->Bind( gfx );
}
