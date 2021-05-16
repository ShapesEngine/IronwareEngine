/*!
 * \file CameraContainer.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "CameraContainer.h"
#include "Graphics.h"
#include "Camera.h"
#include "RenderGraph.h"

#include <imgui/imgui.h>

void CameraContainer::SpawnWindow()
{
	if( ImGui::Begin( "Cameras" ) )
	{
		if( ImGui::BeginCombo( "Active Camera", GetCamera().GetName().c_str() ) )
		{
			for( uint32_t i = 0; i < cameras.size(); i++ )
			{
				const bool isSelected = i == selected;
				if( ImGui::Selectable( cameras[i]->GetName().c_str(), isSelected ) )
				{
					selected = i;
				}
			}
			ImGui::EndCombo();
		}

		GetCamera().SpawnControlWidgets();
	}
	ImGui::End();
}

void CameraContainer::AddCamera( std::unique_ptr<Camera> pCam )
{
	cameras.push_back( std::move( pCam ) );
}

void CameraContainer::Bind( Graphics & gfx )
{
	gfx.SetCamera( GetCamera().GetMatrix() );
}

Camera & CameraContainer::GetCamera()
{
	return *cameras[selected];
}

void CameraContainer::LinkTechniques( RenderGraph& rg )
{
	for( auto& pcam : cameras )
	{
		pcam->LinkTechniques( rg );
	}
}

void CameraContainer::Submit() const
{
	for( size_t i = 0; i < cameras.size(); i++ )
	{
		if( i != selected )
		{
			cameras[i]->Submit();
		}
	}
}
