/*!
 * \class PointLight
 *
 * \brief A class that represents point light that can be set into the scene
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
#include "PointLight.h"
#include "Camera.h"

#include <imgui/imgui.h>

PointLight::PointLight( Graphics& gfx, DirectX::XMFLOAT3A homePos, float radius ) :
	homePos( homePos ),
	mesh( gfx, radius ),
	cbuffer( gfx ),
	pCamera( std::make_shared<Camera>( gfx, "Light", homePos, 0.f, 0.f, true ) )
{
	Reset();
}

void PointLight::SpawnControlWindow() noexcept
{
	if( ImGui::Begin( "Light" ) )
	{
		bool dirtyPos = false;
		const auto dcheck = [&dirtyPos]( bool dirty ) {dirtyPos = dirtyPos || dirty; };
		ImGui::Text( "Position" );
		dcheck( ImGui::SliderFloat( "X", &cbufData.pos.x, -65.f, 65.f, "%.1f" ) );
		dcheck( ImGui::SliderFloat( "Y", &cbufData.pos.y, -65.f, 65.f, "%.1f" ) );
		dcheck( ImGui::SliderFloat( "Z", &cbufData.pos.z, -65.f, 65.f, "%.1f" ) );
		if( dirtyPos )
		{
			pCamera->SetPos( cbufData.pos );
		}

		ImGui::Text( "Intensity/Color" );
		ImGui::SliderFloat( "Intensity", &cbufData.diffuseIntensity, 0.01f, 2.f, "%.2f", 2.f );
		ImGui::ColorEdit3( "Diffuse Color", &cbufData.diffuseColor.x );
		ImGui::ColorEdit3( "Ambient", &cbufData.ambient.x );

		ImGui::Text( "Falloff" );
		ImGui::SliderFloat( "Constant", &cbufData.attConst, 0.05f, 10.f, "%.2f", 4.f );
		ImGui::SliderFloat( "Linear", &cbufData.attLin, 0.0001f, 4.f, "%.4f", 8.f );
		ImGui::SliderFloat( "Quadratic", &cbufData.attQuad, 0.0000001f, 10.f, "%.7f", 10.f );

		if( ImGui::Button( "Reset" ) )
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Submit() const IFNOEXCEPT
{
	const DirectX::XMFLOAT3A color = { cbufData.diffuseColor.x, cbufData.diffuseColor.y, cbufData.diffuseColor.z };
	//mesh.UpdateColor( gfx, color );
	mesh.SetPosition( cbufData.pos );
	mesh.Submit();
}

void PointLight::Bind( Graphics& gfx, DirectX::FXMMATRIX view ) const noexcept
{
	// We have to first transform the pos to the view pos, 
	// as all of our computations are being computed relative to the camera
	// ==============================================================================
	auto dataCopy = cbufData;
	const auto pos = DirectX::XMLoadFloat3( &cbufData.pos );
	DirectX::XMStoreFloat3A( &dataCopy.pos, DirectX::XMVector3Transform( pos, view ) );
	// ------------------------------------------------------------------------------
	cbuffer.Update( gfx, dataCopy );
	cbuffer.Bind( gfx );
}

void PointLight::LinkTechniques( RenderGraph & rg )
{
	mesh.LinkTechniques( rg );
}

void PointLight::Reset() noexcept
{
	cbufData = {
		homePos,
		{ 0.05f, 0.05f, 0.05f },
		{ 1.f, 1.f, 1.f },
		1.f,
		1.f,
		0.045f,
		0.0075f,
	};
}

std::shared_ptr<Camera> PointLight::ShareCamera() const noexcept
{
	return pCamera;
}
