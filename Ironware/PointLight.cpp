/*!
 * \class PointLight
 *
 * \brief 
 *
 * \author Yernar Aldabergenov
 * \date December 2020
 */
#include "PointLight.h"
#include "imgui/imgui.h"

PointLight::PointLight( Graphics& gfx, float radius ) :
	mesh( gfx, radius ),
	cbuffer( gfx )
{}

void PointLight::SpawnControlWindow() noexcept
{
	if( ImGui::Begin( "Light" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X", &pos.x, -60.0f, 60.0f, "%.1f" );
		ImGui::SliderFloat( "Y", &pos.y, -60.0f, 60.0f, "%.1f" );
		ImGui::SliderFloat( "Z", &pos.z, -60.0f, 60.0f, "%.1f" );
		if( ImGui::Button( "Reset" ) )
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Draw( Graphics& gfx ) const noexcept( !IS_DEBUG )
{
	mesh.SetPosition( pos );
	mesh.Draw( gfx );
}
