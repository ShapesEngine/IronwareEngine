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
class RenderGraph;

class CameraContainer
{
public:
	void SpawnWindow( Graphics& gfx );
	void AddCamera( std::unique_ptr<Camera> pCam );
	void Bind( Graphics& gfx );
	Camera* operator->();
	void LinkTechniques( RenderGraph& rg );
	void Submit() const;

private:
	Camera& GetControlledCamera();

private:
	std::vector<std::unique_ptr<Camera>> cameras;
	uint32_t active = 0u;
	uint32_t controlled = 0u;
};