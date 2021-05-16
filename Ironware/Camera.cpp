/*!
 * \file Camera.cpp
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 *
 *
 */
#include "Camera.h"
#include "Graphics.h"
#include "IronMath.h"

#include <imgui/imgui.h>

namespace dx = DirectX;

Camera::Camera( Graphics& gfx, std::string name, DirectX::XMFLOAT3 homePos, float homePitch, float homeYaw, Projection proj ) noexcept :
	name( std::move( name ) ),
	homePos( homePos ),
	homePitch( homePitch ),
	homeYaw( homeYaw ),
	homeProj( proj ),
	projection( proj ),
	indicator( gfx )
{
	Reset();
	indicator.SetPos( pos );
	indicator.SetRotation( { pitch, yaw, 0.f } );
}

void Camera::BindToGraphics( Graphics & gfx ) const
{
	gfx.SetCamera( GetMatrix() );
	gfx.SetProjection( projection.GetMatrix() );
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	const auto forwardVector = dx::XMVector3Transform( dx::XMVectorSet( 0.f, 0.f, 1.f, 0.f ), // world forward vec
		dx::XMMatrixRotationRollPitchYaw( pitch, yaw, 0.f ) );

	const auto camPosition = dx::XMLoadFloat3( &pos );
	const auto camTarget = dx::XMVectorAdd( camPosition, forwardVector );
	return dx::XMMatrixLookAtLH( camPosition, camTarget, dx::XMVectorSet( 0.f, 1.f, 0.f, 0.f ) );
}

void Camera::SpawnControlWidgets() noexcept
{
	ImGui::Text( "Position" );
	ImGui::SliderFloat( "X", &pos.x, -80.f, 80.f );
	ImGui::SliderFloat( "Y", &pos.y, -80.f, 80.f );
	ImGui::SliderFloat( "Z", &pos.z, -80.f, 80.f );
	ImGui::Text( "Orientation" );
	ImGui::SliderAngle( "Pitch", &pitch, 0.995f * -90.f, 0.995f * 90.f );
	ImGui::SliderAngle( "Yaw", &yaw, -180.f, 180.f );
	ImGui::Text( "Movement" );
	ImGui::SliderFloat( "Speed", &translationSpeed, MIN_SPEED_LIMIT, MAX_SPEED_LIMIT );
	projection.RenderWidgets();
	if( ImGui::Button( "Reset" ) )
	{
		Reset();
	}
}

void Camera::Rotate( float dx, float dy ) noexcept
{
	pitch = std::clamp( pitch + dy * rotationSpeed, 0.995f * -PI / 2.f, 0.995f * PI / 2.f );
	yaw = wrap_angle( yaw + dx * rotationSpeed );
	indicator.SetRotation( { pitch, yaw, 0.f } );
}

void Camera::Translate( DirectX::XMFLOAT3 translation ) noexcept
{
	dx::XMStoreFloat3( &translation,
		dx::XMVector3Transform(
			dx::XMLoadFloat3( &translation ),
			dx::XMMatrixRotationRollPitchYaw( pitch, yaw, 0.f ) *
			dx::XMMatrixScaling( translationSpeed, translationSpeed, translationSpeed )
		)
	);

	pos = {
		pos.x + translation.x,
		pos.y + translation.y,
		pos.z + translation.z
	};
	indicator.SetPos( pos );
}

void Camera::Reset() noexcept
{
	translationSpeed = 15.f;
	pos = homePos;
	pitch = homePitch;
	yaw = homeYaw;
	projection = homeProj;
}

void Camera::LinkTechniques( RenderGraph & rg )
{
	indicator.LinkTechniques( rg );
}

void Camera::Submit() const
{
	indicator.Submit();
}
