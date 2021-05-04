#include "ModelWindow.h"

void ModelWindow::ShowWindow( Graphics & gfx, const char * name, const Node & root ) noexcept( !IS_DEBUG )
{
	//if( ImGui::Begin( name ) )
	//{
	//	ImGui::Columns( 2, nullptr, true );
	//	//1st column
	//	root.ShowTree( pSelectedNode );

	//	ImGui::NextColumn();
	//	//2nd column
	//	if( pSelectedNode )
	//	{
	//		const auto id = pSelectedNode->GetID();
	//		auto i = transforms.find( id );
	//		if( i == transforms.end() )
	//		{
	//			TransformationParams tp;
	//			const auto& tf = pSelectedNode->GetAppliedTransform();

	//			const auto rot = extract_euler_angles( tf );
	//			tp.pitch = rot.x;
	//			tp.yaw = rot.y;
	//			tp.roll = rot.z;

	//			const auto translation = extract_translation( tf );
	//			tp.x = translation.x;
	//			tp.y = translation.y;
	//			tp.z = translation.z;

	//			auto pMatConst = pSelectedNode->GetMaterialConstants();
	//			auto buf = pMatConst != nullptr ? std::optional<Buffer>{ *pMatConst } : std::optional<Buffer>{};
	//			std::tie( i, std::ignore ) = transforms.insert( { id, { std::move( tp ), false, std::move( buf ), false } } );
	//		}

	//		auto& transform = i->second;
	//		ImGui::Text( "Position" );
	//		ImGui::SliderFloat( "X", &transform.tranformParams.x, -20.f, 20.f );
	//		ImGui::SliderFloat( "Y", &transform.tranformParams.y, -20.f, 20.f );
	//		ImGui::SliderFloat( "Z", &transform.tranformParams.z, -20.f, 20.f );

	//		ImGui::Text( "Orientation" );
	//		ImGui::SliderAngle( "Pitch", &transform.tranformParams.pitch, -180.f, 180.f );
	//		ImGui::SliderAngle( "Yaw", &transform.tranformParams.yaw, -180.f, 180.f );
	//		ImGui::SliderAngle( "Roll", &transform.tranformParams.roll, -180.f, 180.f );

	//		// link imgui ctrl to cached material params
	//		if( i->second.materialCbuf )
	//		{
	//			auto& mat = *i->second.materialCbuf;
	//			// dirty check
	//			auto& dirty = i->second.materialCbufDirty;
	//			const auto dcheck = [&dirty]( bool changed ) {dirty = dirty || changed; };
	//			// widgets
	//			ImGui::Text( "Material" );
	//			if( auto v = mat["normalMapEnabled"]; v.Exists() )
	//			{
	//				dcheck( ImGui::Checkbox( "Norm Map", &v ) );
	//			}
	//			if( auto v = mat["specularMapEnabled"]; v.Exists() )
	//			{
	//				dcheck( ImGui::Checkbox( "Spec Map", &v ) );
	//			}
	//			if( auto v = mat["hasGlossMap"]; v.Exists() )
	//			{
	//				dcheck( ImGui::Checkbox( "Gloss Map", &v ) );
	//			}
	//			if( auto v = mat["materialColor"]; v.Exists() )
	//			{
	//				dcheck( ImGui::ColorPicker3( "Diff Color", reinterpret_cast<float*>( &static_cast<dx::XMFLOAT3&>( v ) ) ) );
	//			}
	//			if( auto v = mat["specularPower"]; v.Exists() )
	//			{
	//				dcheck( ImGui::SliderFloat( "Spec Power", &v, 0.0f, 100.0f, "%.1f", 1.5f ) );
	//			}
	//			if( auto v = mat["specularColor"]; v.Exists() )
	//			{
	//				dcheck( ImGui::ColorPicker3( "Spec Color", reinterpret_cast<float*>( &static_cast<dx::XMFLOAT3&>( v ) ) ) );
	//			}
	//			if( auto v = mat["specularMapWeight"]; v.Exists() )
	//			{
	//				dcheck( ImGui::SliderFloat( "Spec Weight", &v, 0.0f, 4.0f ) );
	//			}
	//			if( auto v = mat["specularIntensity"]; v.Exists() )
	//			{
	//				dcheck( ImGui::SliderFloat( "Spec Intens", &v, 0.0f, 1.0f ) );
	//			}
	//		}
	//	}
	//	//==========
	//}
	//ImGui::End();
}

void ModelWindow::ApplyParameters() noexcept( !IS_DEBUG )
{
	/*if( TransformDirty() )
	{
	pSelectedNode->SetAppliedTransform( GetTransform() );
	ResetTransformDirty();
	}
	if( MaterialDirty() )
	{
	pSelectedNode->SetMaterialConstants( GetMaterial() );
	ResetMaterialDirty();
	}*/
}

DirectX::XMMATRIX ModelWindow::GetTransform() const noexcept( !IS_DEBUG )
{
	const auto& transform = transforms.at( pSelectedNode->index );
	return dx::XMMatrixRotationRollPitchYaw( transform.tranformParams.pitch, transform.tranformParams.yaw, transform.tranformParams.roll ) *
		dx::XMMatrixTranslation( transform.tranformParams.x, transform.tranformParams.y, transform.tranformParams.z );
}

const Buffer & ModelWindow::GetMaterial() const noexcept( !IS_DEBUG )
{
	assert( pSelectedNode != nullptr );
	const auto& mat = transforms.at( pSelectedNode->GetID() ).materialCbuf;
	assert( mat );
	return *mat;
}
