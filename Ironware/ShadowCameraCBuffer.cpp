/*!
 * \file ShadowCameraCBuffer.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "ShadowCameraCBuffer.h"
#include "Camera.h"

ShadowCameraCBuffer::ShadowCameraCBuffer( Graphics & gfx, UINT slot ) :
	pVcbuf{ std::make_unique<VertexConstantBuffer<Transform>>( gfx, slot ) }
{}

void ShadowCameraCBuffer::Update( Graphics & gfx )
{
	const Transform t{
			DirectX::XMMatrixTranspose(
				pCamera->GetMatrix() * pCamera->GetProjection()
			)
	};
	pVcbuf->Update( gfx, t );
}

void ShadowCameraCBuffer::SetCamera( const Camera * pCamera_in ) noexcept
{
	pCamera = pCamera_in;
}