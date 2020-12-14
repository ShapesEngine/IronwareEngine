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

		ImGui::Text( "Intensity/Color" );
		ImGui::SliderFloat( "Intensity", &cbufData.diffuseIntensity, 0.01f, 2.f, "%.2f", 2 );
		ImGui::ColorEdit3( "Diffuse Color", &cbufData.diffuseColor.x );
		ImGui::ColorEdit3( "Ambient", &cbufData.ambient.x );

		ImGui::Text( "Falloff" );
		ImGui::SliderFloat( "Constant", &cbufData.attConst, 0.05f, 10.f, "%.2f", 4 );
		ImGui::SliderFloat( "Linear", &cbufData.attLin, 0.0001f, 4.f, "%.4f", 8 );
		ImGui::SliderFloat( "Quadratic", &cbufData.attQuad, 0.0000001f, 10.f, "%.7f", 10 );

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
		{ 0.f, 0.f, 0.f },
		{ 0.05f, 0.05f, 0.05f },
		{ 1.f, 1.f, 1.f },
		1.f,
		1.f,
		0.045f,
		0.0075f,
	};
}

void PointLight::Bind( Graphics& gfx ) const noexcept
{ 
	cbuffer.Update( gfx, cbufData );
	cbuffer.Bind( gfx );
}
