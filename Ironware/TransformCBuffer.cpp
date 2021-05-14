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

TransformCBuffer::TransformCBuffer( Graphics & gfx, UINT slot )
{
	if( !pVertConstBuffer )
	{
		pVertConstBuffer = std::make_unique<VertexConstantBuffer<Transforms>>( gfx, slot );
	}
}

void TransformCBuffer::InitializeParentReference( const Drawable & parent ) noexcept
{
	pParent = &parent;
}

void TransformCBuffer::UpdateBind( Graphics& gfx, const Transforms& transforms ) noexcept
{
	assert( pParent );
	pVertConstBuffer->Update( gfx, transforms );
	pVertConstBuffer->Bind( gfx );
}

TransformCBuffer::Transforms TransformCBuffer::GetTransform( Graphics& gfx ) const noexcept
{
	assert( pParent );
	const auto modelView = pParent->GetTransformXM() * gfx.GetCameraXM();
	return {
		// M * V => model * view
		DirectX::XMMatrixTranspose( modelView ),
		// MV * P => modelview * projection
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
		)
	};
}

std::wstring TransformCBuffer::GetUID() const noexcept
{
	static_assert( "Please don't use bindable collection system with this class" );
	return L"?";
}