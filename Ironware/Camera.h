/*!
 * \file Camera.h
 * \date 2020/11/09 7:03
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A header that contains camera class
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "Projection.h"
#include "CameraIndicator.h"
#include "RenderStep.h"

#include <string>

class Graphics;
class RnederGraph;

/*!
 * \class Camera
 *
 * \brief A camera class that is responsible for controlling camera transformations via imgui
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
class Camera
{
public:
	Camera( Graphics& gfx, std::string name, DirectX::XMFLOAT3 homePos = { 0.f, 0.f, 0.f }, float homePitch = 0.f, float homeYaw = 0.f, bool tethered = false ) noexcept;
	void BindToGraphics( Graphics& gfx ) const;
	DirectX::XMMATRIX GetMatrix() const noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void SpawnControlWidgets( Graphics& gfx ) noexcept;
	void Rotate( float dx, float dy ) noexcept;
	void Translate( DirectX::XMFLOAT3 translation ) noexcept;
	void Reset( Graphics& gfx ) noexcept;
	void LinkTechniques( RenderGraph& rg );
	void Submit( size_t channel ) const;

	void SpeedUp() noexcept { translationSpeed += translationSpeed <= MAX_SPEED_LIMIT ? SPEED_MOD_VALUE : 0.f; }
	void SpeedDown() noexcept { translationSpeed -= translationSpeed >= MIN_SPEED_LIMIT ? SPEED_MOD_VALUE : 0.f; }
	void SetPos( DirectX::XMFLOAT3 pos_in ) noexcept;
	const DirectX::XMFLOAT3& GetPos() const noexcept { return pos; }
	const std::string& GetName() const noexcept { return name; }

private:
	bool tethered;
	std::string name;
	DirectX::XMFLOAT3 homePos;
	float homePitch;
	float homeYaw;
	static constexpr float MAX_SPEED_LIMIT = 45.f;
	static constexpr float MIN_SPEED_LIMIT = 2.f;
	static constexpr float SPEED_MOD_VALUE = 1.f;
	bool enableCameraIndicator = true;
	bool enableFrustumIndicator = true;
	float translationSpeed;
	float rotationSpeed = 0.004f;
	DirectX::XMFLOAT3 pos;
	float pitch;
	float yaw;
	Projection projection;
	Projection homeProj;
	CameraIndicator indicator;
};