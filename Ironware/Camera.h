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

#include "Graphics.h"

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
	Camera();	
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Rotate( float dx, float dy ) noexcept;
	void Translate( DirectX::XMFLOAT3 translation ) noexcept;
	void Reset() noexcept;
	void SpeedUp() noexcept { translationSpeed += translationSpeed <= MAX_SPEED_LIMIT ? SPEED_MOD_VALUE : 0.f; }
	void SpeedDown() noexcept { translationSpeed -= translationSpeed >= MIN_SPEED_LIMIT ? SPEED_MOD_VALUE : 0.f; }

private:
	static constexpr float MAX_SPEED_LIMIT = 45.f;
	static constexpr float MIN_SPEED_LIMIT = 2.f;
	static constexpr float SPEED_MOD_VALUE = 1.f;
	float translationSpeed;
	float rotationSpeed = 0.004f;
	DirectX::XMFLOAT3 pos;
	float pitch;
	float yaw;
};

