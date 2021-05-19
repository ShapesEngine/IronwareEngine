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

Camera::Camera( Graphics& gfx, std::string name, DirectX::XMFLOAT3 homePos, float homePitch, float homeYaw, bool tehtered ) noexcept :
	name( std::move( name ) ),
	homePos( homePos ),
	homePitch( homePitch ),
	homeYaw( homeYaw ),
	homeProj( gfx, 1.f, 9.f / 16.f, 0.5f, 500.f ),
	projection( gfx, 1.f, 9.f / 16.f, 0.5f, 500.f ),
	indicator( gfx ),
	tethered( tehtered )
{
	Reset( gfx );
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

void Camera::SpawnControlWidgets( Graphics& gfx ) noexcept
{
	bool rotDirty = false;
	bool posDirty = false;
	const auto dcheck = []( bool d, bool& carry ) { carry = carry || d; };

	if( !tethered )
	{
		ImGui::Text( "Position" );
		dcheck( ImGui::SliderFloat( "X", &pos.x, -80.f, 80.f ), posDirty );
		dcheck( ImGui::SliderFloat( "Y", &pos.y, -80.f, 80.f ), posDirty );
		dcheck( ImGui::SliderFloat( "Z", &pos.z, -80.f, 80.f ), posDirty );
	}

	ImGui::Text( "Orientation" );
	dcheck( ImGui::SliderAngle( "Pitch", &pitch, 0.995f * -90.f, 0.995f * 90.f ), rotDirty );
	dcheck( ImGui::SliderAngle( "Yaw", &yaw, -180.f, 180.f ), rotDirty );
	ImGui::Text( "Movement" );
	ImGui::SliderFloat( "Speed", &translationSpeed, MIN_SPEED_LIMIT, MAX_SPEED_LIMIT );
	projection.RenderWidgets( gfx );
	ImGui::Checkbox( "Camera Indicator", &enableCameraIndicator );
	ImGui::Checkbox( "Frustum Indicator", &enableFrustumIndicator );
	if( ImGui::Button( "Reset" ) )
	{
		Reset( gfx );
	}

	if( rotDirty )
	{
		const dx::XMFLOAT3 angles = { pitch,yaw,0.0f };
		indicator.SetRotation( angles );
		projection.SetRotation( angles );
	}
	if( posDirty )
	{
		indicator.SetPos( pos );
		projection.SetPos( pos );
	}
}

void Camera::Rotate( float dx, float dy ) noexcept
{
	if( !tethered )
	{
		pitch = std::clamp( pitch + dy * rotationSpeed, 0.995f * -PI / 2.f, 0.995f * PI / 2.f );
		yaw = wrap_angle( yaw + dx * rotationSpeed );
		const dx::XMFLOAT3 angles = { pitch, yaw, 0.f };
		indicator.SetRotation( angles );
		projection.SetRotation( angles );
	}
}

void Camera::Translate( DirectX::XMFLOAT3 translation ) noexcept
{
	if( !tethered )
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
		projection.SetPos( pos );
	}
}

void Camera::Reset( Graphics& gfx ) noexcept
{
	translationSpeed = 15.f;
	pos = homePos;
	pitch = homePitch;
	yaw = homeYaw;
	//projection = homeProj;

	const dx::XMFLOAT3 angles = { pitch, yaw, 0.f };
	indicator.SetPos( pos );
	indicator.SetRotation( angles );
	projection.SetPos( pos );
	projection.SetRotation( angles );
	projection.Reset( gfx );
}

void Camera::LinkTechniques( RenderGraph & rg )
{
	indicator.LinkTechniques( rg );
	projection.LinkTechniques( rg );
}

void Camera::Submit() const
{
	if( enableCameraIndicator )
	{
		indicator.Submit();
	}
	if( enableFrustumIndicator )
	{
		projection.Submit();
	}
}

void Camera::SetPos( DirectX::XMFLOAT3 pos_in ) noexcept
{
	pos = pos_in;
	indicator.SetPos( pos );
	projection.SetPos( pos );
}