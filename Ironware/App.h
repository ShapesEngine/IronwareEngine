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
#include "Model.h"
#include "Mesh.h"
#include "Box.h"
#include "FrameExecutor.h"
#include "Material.h"

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
	ImguiManager imguim;
	Camera camera;
	Window wnd{ 1280, 720, L"Ironware Engine" };
	FrameExecutor fexe{ wnd.Gfx() };
	PointLight pointLight{ wnd.Gfx() };
	//std::unique_ptr<class Mesh> pLoaded;
	//Model goblin{ wnd.Gfx(), L"Models\\goblin\\GoblinX.obj", 5.f, { 4.f, 10.f, -16.f } };
	/*Model nano{ wnd.Gfx(), L"Models\\nanosuit_textured\\nanosuit.obj" };
	Model wallObj{ wnd.Gfx(), L"Models\\brickwall\\brickwall.obj", 3.f, { -10.f, 10.f, 0.f } };*/
	/*Sheet sheet1{ wnd.Gfx(), 3.f, { 0.f, 0.f, 1.f, 0.5f } };
	Sheet sheet2{ wnd.Gfx(), 3.f, { 1.f, 0.f, 0.f, 0.5f } };*/
	Model sponza{ wnd.Gfx(), L"Models\\sponza\\sponza.obj", 1.f / 20.f };
	IronTimer timer;
};