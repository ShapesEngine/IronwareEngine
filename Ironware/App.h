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
#include "Box.h"

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
	Box box1{ wnd.Gfx(), 5.f };
	Box box2{ wnd.Gfx(), 5.f };
	/*Model goblin{ wnd.Gfx(), L"Models\\goblin\\GoblinX.obj", 6.f, { 4.f, 10.f, -16.f } };
	Model nano{ wnd.Gfx(), L"Models\\nanosuit_textured\\nanosuit.obj" };
	Model wallObj{ wnd.Gfx(), L"Models\\brickwall\\brickwall.obj", 3.f, { -10.f, 10.f, 0.f } };*/
	/*Sheet sheet1{ wnd.Gfx(), 3.f, { 0.f, 0.f, 1.f, 0.5f } };
	Sheet sheet2{ wnd.Gfx(), 3.f, { 1.f, 0.f, 0.f, 0.5f } };*/
	//Model sponza{ wnd.Gfx(), L"Models\\sponza\\sponza.obj", 1.f / 20.f };
	IronTimer timer;
};