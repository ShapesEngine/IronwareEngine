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

void CameraContainer::SpawnWindow( Graphics& gfx )
{
	if( ImGui::Begin( "Cameras" ) )
	{
		if( ImGui::BeginCombo( "Active Camera", ( *this )->GetName().c_str() ) )
		{
			for( uint32_t i = 0; i < cameras.size(); i++ )
			{
				const bool isSelected = i == active;
				if( ImGui::Selectable( cameras[i]->GetName().c_str(), isSelected ) )
				{
					active = i;
				}
			}
			ImGui::EndCombo();
		}

		if( ImGui::BeginCombo( "Controlled Camera", GetControlledCamera().GetName().c_str() ) )
		{
			for( int i = 0; i < std::size( cameras ); i++ )
			{
				const bool isSelected = i == controlled;
				if( ImGui::Selectable( cameras[i]->GetName().c_str(), isSelected ) )
				{
					controlled = i;
				}
			}
			ImGui::EndCombo();
		}

		GetControlledCamera().SpawnControlWidgets( gfx );
	}
	ImGui::End();
}

void CameraContainer::AddCamera( std::shared_ptr<Camera> pCam )
{
	cameras.push_back( std::move( pCam ) );
}

void CameraContainer::Bind( Graphics & gfx )
{
	gfx.SetCamera( ( *this )->GetMatrix() );
}

Camera * CameraContainer::operator->()
{
	return cameras[active].get();
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
		if( i != active )
		{
			cameras[i]->Submit();
		}
	}
}

Camera & CameraContainer::GetControlledCamera()
{
	return *cameras[controlled];
}