/*!
 * \file Camera.cpp
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 *
 *
 */
#include "Camera.h"
#include "IronMath.h"

#include <imgui/imgui.h>

namespace dx = DirectX;

Camera::Camera()
{
	Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	const auto forwardVector = dx::XMVector3Transform( dx::XMVectorSet( 0.f, 0.f, 1.f, 0.f ), // world forward vec
		dx::XMMatrixRotationRollPitchYaw( pitch, yaw, 0.f ) );

	const auto camPosition = dx::XMLoadFloat3( &pos );
	const auto camTarget = dx::XMVectorAdd( camPosition, forwardVector );
	return dx::XMMatrixLookAtLH( camPosition, camTarget, dx::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ) );
}

void Camera::SpawnControlWindow() noexcept
{
	if( ImGui::Begin( "Camera" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X", &pos.x, -80.f, 80.f );
		ImGui::SliderFloat( "Y", &pos.y, -80.f, 80.f );
		ImGui::SliderFloat( "Z", &pos.z, -80.f, 80.f );
		ImGui::Text( "Orientation" );
		ImGui::SliderAngle( "Pitch", &pitch, 0.995f * -90.f, 0.995f * 90.f );
		ImGui::SliderAngle( "Yaw", &yaw, -180.f, 180.f );
		if( ImGui::Button( "Reset" ) )
		{
			Reset();
		}
	}
	ImGui::End();
}

void Camera::Rotate( float dx, float dy ) noexcept
{
	pitch = std::clamp( pitch + dy * rotationSpeed, 0.995f * -PI / 2.f, 0.995f * PI / 2.f );
	yaw = wrap_angle( yaw + dx * rotationSpeed );
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
}

void Camera::Reset() noexcept
{
	pos = { 0.f, 5.f, -28.f };
	pitch = 0.f;
	yaw = 0.f;
}
