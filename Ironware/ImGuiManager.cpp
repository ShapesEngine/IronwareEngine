/*!
 * \class ImGuiManager
 *
 * \brief A manager class for creating and configuring(setting styles, etc..) imgui
 *
 * \author Yernar Aldabergenov
 * \date December 2020
 */
#include "ImGuiManager.h"
#include "imgui/imgui.h"

ImguiManager::ImguiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

ImguiManager::~ImguiManager()
{
	ImGui::DestroyContext();
}
