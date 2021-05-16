/*!
 * \file Projection.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "Projection.h"

#include <imgui/imgui.h>

Projection::Projection( float width, float height, float nearZ, float farZ ) :
	width( width ),
	height( height ),
	nearZ( nearZ ),
	farZ( farZ )
{}

void Projection::RenderWidgets()
{
	ImGui::Text( "Projection" );
	ImGui::SliderFloat( "Width", &width, 0.01f, 4.0f, "%.2f", 1.5f );
	ImGui::SliderFloat( "Height", &height, 0.01f, 4.0f, "%.2f", 1.5f );
	ImGui::SliderFloat( "Near Z", &nearZ, 0.01f, 400.0f, "%.2f", 10.0f );
	ImGui::SliderFloat( "Far Z", &farZ, 0.01f, 500.0f, "%.2f", 10.0f );
}
