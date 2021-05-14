/*!
 * \file CameraContainer.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "Camera.h"

#include <vector>
#include <memory>

class Graphics;

class CameraContainer
{
public:
	void SpawnWindow();
	void AddCamera( std::unique_ptr<Camera> pCam );
	void Bind( Graphics& gfx );
	Camera& GetCamera();

private:
	std::vector<std::unique_ptr<Camera>> cameras;
	uint32_t selected = 0u;
};