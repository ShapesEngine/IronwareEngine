#if 0
/*!
 * \file TransformCBufferEx.cpp
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#include "TransformCBufferEx.h"

std::unique_ptr<PixelConstantBuffer<TransformCBufferEx::Transforms>> TransformCBufferEx::pPixConstBuffer;

TransformCBufferEx::TransformCBufferEx( Graphics& gfx, const Drawable& parent, UINT slotVS, UINT slotPS ) :
	TransformCBuffer( gfx, parent, slotVS )
{
	if( !pPixConstBuffer )
	{
		pPixConstBuffer = std::make_unique<PixelConstantBuffer<Transforms>>( gfx, slotPS );
	}
}

void TransformCBufferEx::Bind( Graphics & gfx ) noexcept
{
	const auto transform = GetTransform( gfx );
	TransformCBuffer::UpdateBind( gfx, transform );
	UpdateBind( gfx, transform );
}

void TransformCBufferEx::UpdateBind( Graphics& gfx, const Transforms& transforms ) noexcept
{
	pPixConstBuffer->Update( gfx, transforms );
	pPixConstBuffer->Bind( gfx );
}
#endif // 0
