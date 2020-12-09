/*!
 * \file Camera.h
 * \date 2020/12/09 7:03
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A camera class that is responsible for controlling camera transformations via imgui
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "Graphics.h"

class Camera
{
public:
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;

private:
	float zOffset = 20.f;
	float theta = 0.f;
	float phi = 0.f;
	float pitch = 0.f;
	float yaw = 0.f;
	float roll = 0.f;
};

