/*!
 * \file ImGuiManager.cpp
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 *
 * 
 */
#include "ImGuiManager.h"
#include "imgui/imgui.h"

size_t ImguiManager::refCount = 0;

ImguiManager::ImguiManager()
{
	++refCount;

	if( refCount > 1 )
	{
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

ImguiManager::~ImguiManager()
{
	--refCount;

	if( refCount == 0 )
	{
		ImGui::DestroyContext();
	}	
}
