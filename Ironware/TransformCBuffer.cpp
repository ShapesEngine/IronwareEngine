/*!
 * \class TransformCBuffer
 *
 * \brief A Transform Constant Buffer wrapper child class that will be bound in process of graphics pipeline.
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
#include "TransformCBuffer.h"

std::unique_ptr<VertexConstantBuffer<TransformCBuffer::Transforms>> TransformCBuffer::pVertConstBuffer;

TransformCBuffer::TransformCBuffer( Graphics& gfx, const Drawable& parent ) :
	parent( parent )
{
	if( !pVertConstBuffer )
	{
		pVertConstBuffer = std::make_unique<VertexConstantBuffer<Transforms>>( gfx );
	}
}

 void TransformCBuffer::Bind( Graphics & gfx ) noexcept
{
	const auto model = parent.GetTransformXM();
	const Transforms transforms =
	{
		DirectX::XMMatrixTranspose( model ),
		DirectX::XMMatrixTranspose(
			model *
			gfx.GetCamera() *
			gfx.GetProjection()
		)
	};
	// M * V * P => model * view * projection
	pVertConstBuffer->Update( gfx, transforms );
	pVertConstBuffer->Bind( gfx );
}
