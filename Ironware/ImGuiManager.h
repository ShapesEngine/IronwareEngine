/*!
 * \file ImGuiManager.h
 * \date 2020/11/08 20:32
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A manager class for creating and configuring(setting styles, etc..) imgui
 *
 * TODO:
 *
 * \note
*/
#pragma once

/*!
 * \class ImGuiManager
 *
 * \brief A manager class for creating and configuring(setting styles, etc..) imgui
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
class ImguiManager
{
public:
	ImguiManager();
	~ImguiManager();

private:
	static size_t refCount;
};
