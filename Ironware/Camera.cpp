/*!
 * \file Camera.cpp
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 *
 * 
 */
#include "Camera.h"
#include "imgui/imgui.h"

namespace dx = DirectX;

Camera::Camera()
{
	Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	const auto position = dx::XMVector3Transform(
		dx::XMVectorSet( 0.f, 0.f, zOffset, 0.f ),
		dx::XMMatrixRotationRollPitchYaw( phi, -theta, 0.f )
	);
	return dx::XMMatrixLookAtLH( position, dx::XMVectorZero(),
		// world up vector
		dx::XMVectorSet( 0.f, 1.f, 0.f, 0.f ) ) * 
		dx::XMMatrixRotationRollPitchYaw( pitch, -yaw, roll );
}

void Camera::SpawnControlWindow() noexcept
{
	if( ImGui::Begin( "Camera" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "Z Offset", &zOffset, 0.f, 80.f, "%.1f" );
		ImGui::SliderAngle( "Y Rotation", &theta, -180.f, 180.f );
		ImGui::SliderAngle( "X Rotation", &phi, -89.f, 89.f );
		ImGui::Text( "Orientation" );
		ImGui::SliderAngle( "Roll", &roll, -180.f, 180.f );
		ImGui::SliderAngle( "Pitch", &pitch, -180.f, 180.f );
		ImGui::SliderAngle( "Yaw", &yaw, -180.f, 180.f );
		if( ImGui::Button( "Reset" ) )
		{
			Reset();
		}
	}
	ImGui::End();
}

void Camera::Reset() noexcept
{
	zOffset = 20.f;
	theta = 0.f;
	phi = 0.f;
	pitch = 0.f;
	yaw = 0.f;
	roll = 0.f;
}
