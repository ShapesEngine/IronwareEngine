/*!
 * \file TransformCBufferScaling.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#include "TransformCBufferScaling.h"
#include "TechniqueProbe.h"

namespace dx = DirectX;

TransformCBufferScaling::TransformCBufferScaling( Graphics& gfx, float scale ) :
	TransformCBuffer( gfx ),
	buf( MakeLayout() )
{
	buf["scale"] = scale;
}

void TransformCBufferScaling::Accept( TechniqueProbe& probe )
{
	probe.VisitBuffer( buf );
}

void TransformCBufferScaling::Bind( Graphics& gfx ) noexcept
{
	const float scale = buf["scale"];
	const auto scaleMatrix = dx::XMMatrixScaling( scale, scale, scale );
	auto xf = GetTransform( gfx );
	xf.modelView = xf.modelView * scaleMatrix;
	xf.modelViewProj = xf.modelViewProj * scaleMatrix;
	UpdateBind( gfx, xf );
}

std::unique_ptr<CloningBindable> TransformCBufferScaling::Clone() const noexcept
{
	return std::make_unique<TransformCBufferScaling>( *this );
}

RawLayout TransformCBufferScaling::MakeLayout()
{
	RawLayout layout;
	layout.Add<Float>( "scale" );
	return layout;
}