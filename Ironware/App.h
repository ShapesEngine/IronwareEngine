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
/**
 * @brief 
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

private:
	ImguiManager imguim;
	Camera camera;
	Window wnd;
	PointLight pointLight;
};
