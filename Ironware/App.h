/*!
 * \file App.h
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#pragma once

#include "Window.h"
#include "ImGuiManager.h"
#include "IronTimer.h"
#include "Camera.h"
#include "PointLight.h"
#include "Model.h"

 /**
  * @brief Base class that controls scene
 */
class App
{
public:
	App();

	/**
	 * @brief Starts application frame. Processes window messages
	 * @return Integer exit code
	*/
	int BeginFrame();

private:
	/**
	 * @brief Processes main application frame
	*/
	void ProcessFrame();
	void HandleInput();
	void ToggleCursor() { wnd.IsCursorEnabled() ? wnd.DisableMouseCursor() : wnd.EnableMouseCursor(); }

private:
	int x = 0, y = 0;
	ImguiManager imguim;
	Camera camera;
	Window wnd{ 1280, 720, L"Ironware Engine" };
	PointLight pointLight;
	Model nano{ wnd.Gfx(), "Models\\nanosuit_h.gltf" };
	IronTimer timer;
};