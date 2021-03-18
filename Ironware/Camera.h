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
	void Reset() noexcept;

private:
	float zOffset;
	float theta;
	float phi;
	float pitch;
	float yaw;
	float roll;
};

