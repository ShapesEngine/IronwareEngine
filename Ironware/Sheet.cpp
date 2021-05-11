/*!
 * \file Sheet.cpp
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#include "Sheet.h"
#include "Vertex.h"
#define IR_INCLUDE_TEXTURE
#include "BindableCommon.h"
#include <imgui/imgui.h>
#include "Plane.h"

Sheet::Sheet( Graphics& gfx, float size, DirectX::XMFLOAT4 color ) :
	cbuff{ color }
{
	Reset();

	namespace dx = DirectX;

	struct Vertex
	{
		dx::XMFLOAT3 pos;
		dx::XMFLOAT3 n;
		dx::XMFLOAT2 tex;
	};

	auto model = Plane::Make<Vertex>();
	model.Transform( DirectX::XMMatrixScaling( size, size, 1.f ) );
	VertexByteBuffer vbuff(
		VertexLayout{}
		.Append( VertexLayout::ElementType::Position3D )
		.Append( VertexLayout::ElementType::Normal )
	);
	for( auto& v : model.vertices )
	{
		vbuff.EmplaceBack( v.pos, v.n );
	}
	const std::wstring& sheetTag = L"$sheet." + std::to_wstring( size );
	AddBind( VertexBuffer::Resolve( gfx, sheetTag, vbuff ) );
	AddBind( IndexBuffer::Resolve( gfx, sheetTag, model.indices ) );

	auto pVertexShader = VertexShader::Resolve( gfx, L"SolidVS.cso" );
	auto pVertexShaderBytecode = pVertexShader->GetBytecode();
	AddBind( std::move( pVertexShader ) );

	AddBind( PixelShader::Resolve( gfx, L"SolidPS.cso" ) );

	AddBind( Sampler::Resolve( gfx ) );

	AddBind( std::make_shared<PixelConstantBuffer<BoxCBuff>>( gfx, cbuff, 1u ) );

	AddBind( InputLayout::Resolve( gfx, vbuff.GetLayout(), pVertexShaderBytecode ) );

	AddBind( PrimitiveTopology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	AddBind( std::make_shared<TransformCBufferEx>( gfx, *this, 0u, 2u ) );

	AddBind( std::make_shared<BlendState>( gfx, true, 0.5f ) );

	AddBind( RasterizerState::Resolve( gfx, true ) );
}

DirectX::XMMATRIX Sheet::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw( orientation.x, orientation.y, orientation.z ) *
		DirectX::XMMatrixTranslation( pos.x, pos.y, pos.z );
}

void Sheet::SpawnControlWindow( Graphics& gfx, const char* pName ) noexcept
{
	if( ImGui::Begin( pName ? pName : "Sheet" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X", &pos.x, -60.f, 60.f, "%.1f" );
		ImGui::SliderFloat( "Y", &pos.y, -60.f, 60.f, "%.1f" );
		ImGui::SliderFloat( "Z", &pos.z, -60.f, 60.f, "%.1f" );

		ImGui::Text( "Orientation" );
		ImGui::SliderAngle( "Pitch", &orientation.x, -180.f, 180.f );
		ImGui::SliderAngle( "Yaw", &orientation.y, -180.f, 180.f );
		ImGui::SliderAngle( "Roll", &orientation.z, -180.f, 180.f );

		ImGui::Text( "Properties" );

		auto pBlender = QueryBindable<BlendState>();
		auto factor = pBlender->GetFactor();

		ImGui::SliderFloat( "Factor", &factor, 0.f, 1.f );

		pBlender->SetFactor( factor );

		if( ImGui::Button( "Reset" ) )
		{
			Reset();
		}
	}
	ImGui::End();
}

void Sheet::Reset() noexcept
{
	pos = { 10.f, 10.f, 0.f };
	orientation = { 0.f, 0.f, 0.f };
}