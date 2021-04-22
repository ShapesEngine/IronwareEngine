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
#include "GDIPlusManager.h"
#include "Sheet.h"

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

private:
	GDIPlusManager gdipm;
	ImguiManager imguim;
	Camera camera;
	Window wnd{ 1280, 720, L"Ironware Engine" };
	PointLight pointLight{ wnd.Gfx() };
	Model goblin{ wnd.Gfx(), "Models\\goblin\\GoblinX.obj", { 4.f, 10.f, -6.f } };
	//Model nano{ wnd.Gfx(), "Models\\nanosuit_textured\\nanosuit.obj" };
	//Sheet wall{ wnd.Gfx(), 3.f };
	IronTimer timer;
};