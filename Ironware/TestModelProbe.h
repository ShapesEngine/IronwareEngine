/*!
 * \file TestModelProbe.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once
#include "TechniqueProbe.h"
#include "ModelProbe.h"
#include "imgui/imgui.h"
#include "RenderTechnique.h"
#include "DynamicConstantBuffer.h"
#include "Model.h"
#include "Node.h"
#include <DirectXMath.h>
#include <string>
#include <unordered_map>
#include "IronMath.h"

namespace dx = DirectX;

// Mesh techniques window
class TP : public TechniqueProbe
{
public:
	void OnSetTechnique() override
	{
		using namespace std::string_literals;
		ImGui::TextColored( { 0.4f,1.f,0.6f,1.f }, to_narrow( pTech->GetName() ).c_str() );
		bool active = pTech->IsActive();
		ImGui::Checkbox( ( "Tech Active##"s + std::to_string( techIdx ) ).c_str(), &active );
		pTech->SetActive( active );
	}
	bool OnVisitBuffer( Buffer& buf ) override
	{
		namespace dx = DirectX;
		float dirty = false;
		const auto dcheck = [&dirty]( bool changed ) {dirty = dirty || changed; };
		auto tag = [tagScratch = std::string{}, tagString = "##" + std::to_string( bufIdx )]
		( const char* label ) mutable
		{
			tagScratch = label + tagString;
			return tagScratch.c_str();
		};

		if( auto v = buf["scale"]; v.Exists() )
		{
			dcheck( ImGui::SliderFloat( tag( "Scale" ), &v, 1.f, 2.f, "%.3f", 3.5f ) );
		}
		if( auto v = buf["offset"]; v.Exists() )
		{
			dcheck( ImGui::SliderFloat( tag( "offset" ), &v, 0.f, 1.f, "%.3f", 2.5f ) );
		}
		if( auto v = buf["materialColor"]; v.Exists() )
		{
			dcheck( ImGui::ColorPicker3( tag( "Color" ), reinterpret_cast<float*>( &static_cast<dx::XMFLOAT3&>( v ) ) ) );
		}
		if( auto v = buf["specularColor"]; v.Exists() )
		{
			dcheck( ImGui::ColorPicker3( tag( "Spec. Color" ), reinterpret_cast<float*>( &static_cast<dx::XMFLOAT3&>( v ) ) ) );
		}
		if( auto v = buf["specularGloss"]; v.Exists() )
		{
			dcheck( ImGui::SliderFloat( tag( "Glossiness" ), &v, 1.f, 100.f, "%.1f", 1.5f ) );
		}
		if( auto v = buf["specularWeight"]; v.Exists() )
		{
			dcheck( ImGui::SliderFloat( tag( "Spec. Weight" ), &v, 0.f, 2.f ) );
		}
		if( auto v = buf["useSpecularMap"]; v.Exists() )
		{
			dcheck( ImGui::Checkbox( tag( "Spec. Map Enable" ), &v ) );
		}
		if( auto v = buf["useNormalMap"]; v.Exists() )
		{
			dcheck( ImGui::Checkbox( tag( "Normal Map Enable" ), &v ) );
		}
		if( auto v = buf["normalMapWeight"]; v.Exists() )
		{
			dcheck( ImGui::SliderFloat( tag( "Normal Map Weight" ), &v, 0.f, 2.f ) );
		}
		return dirty;
	}
};

class MP : ModelProbe
{
public:
	MP( std::string name ) :
		name( name )
	{}
	void SpawnWindow( Model& model )
	{
		ImGui::Begin( "Model" );
		ImGui::Columns( 2, nullptr, true );
		model.Accept( *this );

		ImGui::NextColumn();
		if( pSelectedNode != nullptr )
		{
			bool dirty = false;
			const auto dcheck = [&dirty]( bool changed ) {dirty = dirty || changed; };
			auto& tf = ResolveTransform();
			ImGui::TextColored( { 0.4f,1.f,0.6f,1.f }, "Translation" );
			dcheck( ImGui::SliderFloat( "X", &tf.x, -60.f, 60.f ) );
			dcheck( ImGui::SliderFloat( "Y", &tf.y, -60.f, 60.f ) );
			dcheck( ImGui::SliderFloat( "Z", &tf.z, -60.f, 60.f ) );
			ImGui::TextColored( { 0.4f,1.f,0.6f,1.f }, "Orientation" );
			dcheck( ImGui::SliderAngle( "X-rotation", &tf.xRot, -180.f, 180.f ) );
			dcheck( ImGui::SliderAngle( "Y-rotation", &tf.yRot, -180.f, 180.f ) );
			dcheck( ImGui::SliderAngle( "Z-rotation", &tf.zRot, -180.f, 180.f ) );
			if( dirty )
			{
				pSelectedNode->SetAppliedTransform(
					dx::XMMatrixRotationX( tf.xRot ) *
					dx::XMMatrixRotationY( tf.yRot ) *
					dx::XMMatrixRotationZ( tf.zRot ) *
					dx::XMMatrixTranslation( tf.x, tf.y, tf.z )
				);
			}

			TP probe;
			pSelectedNode->Accept( probe );
		}
		ImGui::End();
	}
protected:
	bool PushNode( Node& node ) override
	{
		// if there is no selected node, set selectedId to an impossible value
		const int selectedId = ( pSelectedNode == nullptr ) ? -1 : pSelectedNode->GetID();
		// build up flags for current node
		const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
			| ( ( node.GetID() == selectedId ) ? ImGuiTreeNodeFlags_Selected : 0 )
			| ( node.HasChildren() ? 0 : ImGuiTreeNodeFlags_Leaf );
		// render this node
		const auto expanded = ImGui::TreeNodeEx(
			(void*)(intptr_t)node.GetID(),
			node_flags, node.GetName().c_str()
		);
		// processing for selecting node
		if( ImGui::IsItemClicked() )
		{
			pSelectedNode = &node;
		}
		// signal if children should also be recursed
		return expanded;
	}
	void PopNode( Node& node ) override
	{
		ImGui::TreePop();
	}
private:
	Node* pSelectedNode = nullptr;
	struct TransformParameters
	{
		float xRot = 0.f;
		float yRot = 0.f;
		float zRot = 0.f;
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
	};
	std::string name;
	std::unordered_map<int, TransformParameters> transformParams;
private:
	TransformParameters& ResolveTransform() noexcept
	{
		const auto id = pSelectedNode->GetID();
		auto i = transformParams.find( id );
		if( i == transformParams.end() )
		{
			return LoadTransform( id );
		}
		return i->second;
	}
	TransformParameters& LoadTransform( int id ) noexcept
	{
		const auto& applied = pSelectedNode->GetAppliedTransform();
		const auto angles = extract_euler_angles( applied );
		const auto translation = extract_translation( applied );
		TransformParameters tp;
		tp.zRot = angles.z;
		tp.xRot = angles.x;
		tp.yRot = angles.y;
		tp.x = translation.x;
		tp.y = translation.y;
		tp.z = translation.z;
		return transformParams.insert( { id,{ tp } } ).first->second;
	}
};