/*!
 * \file Mesh.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#include "Mesh.h"
#include "Material.h"

Mesh::Mesh( Graphics & gfx, const Material & mat, const aiMesh & mesh ) noexcept( !IS_DEBUG ) :
	Drawable( gfx, mat, mesh )
{}

void Mesh::Submit( FrameExecutor& frame, DirectX::FXMMATRIX accumulatedTransform ) const IFNOEXCEPT
{
	DirectX::XMStoreFloat4x4( &transform, accumulatedTransform );
	Drawable::Submit( frame );
}