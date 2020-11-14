/*!
 * \file App.h
 * \date 2020/11/14 13:02
 *
 * \author Magic
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

class App
{
public:
	App();
	// master frame / message loop
	int Begin();

private:
	void Tick();

private:
	Window wnd;
};


