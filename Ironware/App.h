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
#include "Camera.h"
#include "PointLight.h"
#include "Model.h"

 /**
  * @brief Base class that controls scene
 */
class App
{
	struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;

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

	void ShowRawMouseWindow();
	void ShowModelWindow();

private:
	int x = 0, y = 0;
	ImguiManager imguim;
	Camera camera;
	Window wnd;
	PointLight pointLight;
	Model nano{ wnd.Gfx(), "Models\\nanosuit.obj" };
};