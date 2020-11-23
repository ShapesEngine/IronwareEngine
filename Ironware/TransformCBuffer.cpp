/*!
 * \class TransformCBuffer
 *
 * \brief 
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
#include "TransformCBuffer.h"

TransformCBuffer::TransformCBuffer( Graphics& gfx, const Drawable& parent ) :
	vcbuf( gfx ),
	parent( parent )
{}

inline void TransformCBuffer::Bind( Graphics & gfx ) noexcept
{
	vcbuf.Update( gfx, DirectX::XMMatrixTranspose( parent.GetTransformXM() * gfx.GetProjection() ) );
	vcbuf.Bind( gfx );
}
