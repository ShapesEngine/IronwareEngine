/*!
 * \file App.h
 * \date 2020/11/14 13:02
 *
 * \author Magic
 * Contact: yernar.aa@gmail.com
 *
 * \brief A basic application class that is responsible for application specifig code managing
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "Window.h"

class App
{
public:
	App();
	// master frame / message loop
	int Go();

private:
	void DoFrame();

private:
	Window wnd;
};


