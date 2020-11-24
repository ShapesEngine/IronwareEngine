/*!
 * \class TransformCBuffer
 *
 * \brief A Transform Constant Buffer wrapper child class that will be bound in process of graphics pipeline.
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
#include "TransformCBuffer.h"

TransformCBuffer::TransformCBuffer( Graphics& gfx, const Drawable& parent ) :
	vertConstBuffer( gfx ),
	parent( parent )
{}

inline void TransformCBuffer::Bind( Graphics & gfx ) noexcept
{
	vertConstBuffer.Update( gfx, DirectX::XMMatrixTranspose( parent.GetTransformXM() * gfx.GetProjection() ) );
	vertConstBuffer.Bind( gfx );
}
