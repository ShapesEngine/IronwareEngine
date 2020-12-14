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
{
	Reset();
}

void PointLight::SpawnControlWindow() noexcept
{
	if( ImGui::Begin( "Light" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X", &cbufData.pos.x, -60.f, 60.f, "%.1f" );
		ImGui::SliderFloat( "Y", &cbufData.pos.y, -60.f, 60.f, "%.1f" );
		ImGui::SliderFloat( "Z", &cbufData.pos.z, -60.f, 60.f, "%.1f" );
		if( ImGui::Button( "Reset" ) )
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Draw( Graphics& gfx ) const noexcept( !IS_DEBUG )
{
	mesh.SetPosition( cbufData.pos );
	mesh.Draw( gfx );	
}

void PointLight::Reset() noexcept
{
	cbufData = {
		{ 0.0f,0.0f,0.0f },
		{ 0.7f,0.7f,0.9f },
		{ 0.05f,0.05f,0.05f },
		{ 1.0f,1.0f,1.0f },
		1.0f,
		1.0f,
		0.045f,
		0.0075f,
	};
}

void PointLight::Bind( Graphics& gfx ) const noexcept
{ 
	cbuffer.Update( gfx, cbufData );
	cbuffer.Bind( gfx );
}
