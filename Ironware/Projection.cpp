/*!
 * \file Projection.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "Projection.h"
#include "Graphics.h"
#include "RenderGraph.h"

#include <imgui/imgui.h>

Projection::Projection( Graphics& gfx, float width, float height, float nearZ, float farZ ) :
	width( width ),
	height( height ),
	nearZ( nearZ ),
	farZ( farZ ),
	indicator( gfx, width, height, nearZ, farZ ),
	homeWidth( width ),
	homeHeight( height ),
	homeNearZ( nearZ ),
	homeFarZ( farZ )
{}

void Projection::RenderWidgets( Graphics& gfx )
{
	bool dirty = false;
	const auto dcheck = [&dirty]( bool d ) { dirty = dirty || d; };

	ImGui::Text( "Projection" );
	dcheck( ImGui::SliderFloat( "Width", &width, 0.01f, 4.f, "%.2f", 1.5f ) );
	dcheck( ImGui::SliderFloat( "Height", &height, 0.01f, 4.f, "%.2f", 1.5f ) );
	dcheck( ImGui::SliderFloat( "Near Z", &nearZ, 0.01f, farZ - FLT_EPSILON, "%.2f", 10.f ) );
	dcheck( ImGui::SliderFloat( "Far Z", &farZ, nearZ + FLT_EPSILON, 500.f, "%.2f", 10.f ) );

	if( dirty )
	{
		indicator.SetVertices( gfx, width, height, nearZ, farZ );
	}
}

void Projection::Submit( size_t channelFilter ) const
{
	indicator.Submit( channelFilter );
}

void Projection::LinkTechniques( RenderGraph & rg )
{
	indicator.LinkTechniques( rg );
}

void Projection::Reset( Graphics & gfx )
{
	width = homeWidth;
	height = homeHeight;
	nearZ = homeNearZ;
	farZ = homeFarZ;
	indicator.SetVertices( gfx, width, height, nearZ, farZ );
}
