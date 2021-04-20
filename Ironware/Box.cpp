/*!
 * \file Box.cpp
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 * 
 */
#include "Box.h"
#include "Vertex.h"
#define IR_INCLUDE_TEXTURE
#include "BindableCommon.h"
#include <imgui/imgui.h>
#include "Cube.h"

Box::Box( Graphics & gfx, float size )
{
	Reset();

	namespace dx = DirectX;

	struct Vertex
	{
		dx::XMFLOAT3 pos;
		dx::XMFLOAT3 n;
		dx::XMFLOAT2 tex;
	};

	auto model = Cube::MakeIndependentTextured<Vertex>();
	model.Transform( DirectX::XMMatrixScaling( size, size, size ) );
	model.SetNormalsIndependentFlat();

	VertexByteBuffer vbuff(
		VertexLayout{}
		.Append( VertexLayout::ElementType::Position3D )
		.Append( VertexLayout::ElementType::Normal )
		.Append( VertexLayout::ElementType::Texture2D )
	);
	for( auto& v : model.vertices )
	{
		vbuff.EmplaceBack( v.pos, v.n, v.tex );
	}
	const std::wstring& sheetTag = L"$box." + std::to_wstring( size );
	AddBind( VertexBuffer::Resolve( gfx, sheetTag, vbuff ) );
	AddBind( IndexBuffer::Resolve( gfx, sheetTag, model.indices ) );

	auto pVertexShader = VertexShader::Resolve( gfx, L"PhongDiffuseMapVS.cso" );
	auto pVertexShaderBytecode = pVertexShader->GetBytecode();
	AddBind( std::move( pVertexShader ) );

	AddBind( PixelShader::Resolve( gfx, L"PhongNormalMapPS.cso" ) );

	AddBind( Texture::Resolve( gfx, L"Images\\brickwall.jpg" ) );
	AddBind( Texture::Resolve( gfx, L"Images\\brickwall_normal.jpg", 1u ) );
	AddBind( Sampler::Resolve( gfx ) );

	AddBind( PixelConstantBuffer<SheetCBuff>::Resolve( gfx, cbuff, 1u ) );

	AddBind( InputLayout::Resolve( gfx, vbuff.GetLayout(), pVertexShaderBytecode ) );

	AddBind( PrimitiveTopology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	AddBind( std::make_shared<TransformCBufferEx>( gfx, *this, 0u, 2u ) );
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw( orientation.x, orientation.y, orientation.z ) *
		DirectX::XMMatrixTranslation( pos.x, pos.y, pos.z );
}

void Box::SpawnControlWindow( Graphics & gfx ) noexcept
{
	if( ImGui::Begin( "Box" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X", &pos.x, -60.f, 60.f, "%.1f" );
		ImGui::SliderFloat( "Y", &pos.y, -60.f, 60.f, "%.1f" );
		ImGui::SliderFloat( "Z", &pos.z, -60.f, 60.f, "%.1f" );

		ImGui::Text( "Orientation" );
		ImGui::SliderAngle( "Pitch", &orientation.x, -180.0f, 180.0f );
		ImGui::SliderAngle( "Yaw", &orientation.y, -180.0f, 180.0f );
		ImGui::SliderAngle( "Roll", &orientation.z, -180.0f, 180.0f );

		ImGui::Text( "Properties" );
		const bool bi = ImGui::SliderFloat( "Specular Intensity", &cbuff.specularIntensity, 0.01f, 1.f, "%.2f", 2 );
		const bool bp = ImGui::SliderFloat( "Specular Power", &cbuff.specularPower, 10.f, 50.f, "%.f", 2 );
		const bool bnm = ImGui::Checkbox( "Enable Normal Mapping", reinterpret_cast<bool*>( &cbuff.isNormalMappingEnabled ) );

		if( bi || bp || bnm )
		{
			QueryBindable<PixelConstantBuffer<SheetCBuff>>()->Update( gfx, cbuff );
		}

		if( ImGui::Button( "Reset" ) )
		{
			Reset();
		}
	}
	ImGui::End();
}

void Box::Reset() noexcept
{
	pos = { 10.f, 10.f, 0.f };
	orientation = { 0.f, 0.f, 0.f };
	cbuff.specularIntensity = 0.1f;
	cbuff.specularPower = 22.f;
	cbuff.isNormalMappingEnabled = TRUE;
}