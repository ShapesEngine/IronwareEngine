/*!
 * \file App.h
 * \date 2020/11/14 13:02
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A basic application class that is responsible for application specific code managing
 *
 * TODO:
 *
 * \note For some testing purposes you can inherit from this class and make tests in separate class.
 * * Make sure you make necessary functions virtual
*/
#pragma once

#include "Window.h"
#include "IronTimer.h"
#include "Box.h"
#include "Camera.h"
#include "ImGuiManager.h"

class App
{
public:
	App();

	// master frame / message loop
	int Begin();

private:
	void SetupFrame();

private:
	ImguiManager imgui;
	Camera camera;
	Window wnd;	
	IronTimer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	float simulation_speed_factor = 1.f;
	bool isSimulationRunning = true;
	static constexpr size_t MAX_NDRAWABLES = 180;
};


