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
	const std::wstring& boxTag = L"$box." + std::to_wstring( size );
	AddBind( VertexBuffer::Resolve( gfx, boxTag, vbuff ) );
	AddBind( IndexBuffer::Resolve( gfx, boxTag, model.indices ) );

	auto pVertexShader = VertexShader::Resolve( gfx, L"PhongDiffuseMapVS.cso" );
	auto pVertexShaderBytecode = pVertexShader->GetBytecode();
	AddBind( std::move( pVertexShader ) );

	AddBind( PixelShader::Resolve( gfx, L"PhongDiffuseMapPS.cso" ) );

	AddBind( Texture::Resolve( gfx, L"Images\\brickwall.jpg" ) );
	/*AddBind( Texture::Resolve( gfx, L"Images\\brickwall_normal.jpg", 1u ) );*/
	AddBind( Sampler::Resolve( gfx ) );

	AddBind( PixelConstantBuffer<BoxCBuff>::Resolve( gfx, cbuff, 1u ) );

	AddBind( InputLayout::Resolve( gfx, vbuff.GetLayout(), pVertexShaderBytecode ) );

	AddBind( PrimitiveTopology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	AddBind( std::make_shared<DepthStencilState>( gfx, DepthStencilState::StencilMode::Write ) );

	auto pTfCbuf = std::make_shared<TransformCBufferEx>( gfx, *this, 0u, 2u );
	AddBind( pTfCbuf );

	outlineBindables.push_back( VertexBuffer::Resolve( gfx, boxTag, vbuff ) );
	outlineBindables.push_back( IndexBuffer::Resolve( gfx, boxTag, model.indices ) );
	pVertexShader = VertexShader::Resolve( gfx, L"SolidVS.cso" );
	pVertexShaderBytecode = pVertexShader->GetBytecode();
	outlineBindables.push_back( std::move( pVertexShader ) );
	outlineBindables.push_back( PixelShader::Resolve( gfx, L"SolidPS.cso" ) );
	struct SolidColorBuffer
	{
		DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 0.5f, 1.0f };
	} scb;
	outlineBindables.push_back( PixelConstantBuffer<SolidColorBuffer>::Resolve( gfx, scb, 1u ) );
	outlineBindables.push_back( InputLayout::Resolve( gfx, vbuff.GetLayout(), pVertexShaderBytecode ) );
	outlineBindables.push_back( PrimitiveTopology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	outlineBindables.push_back( std::move( pTfCbuf ) );
	outlineBindables.push_back( std::make_shared<DepthStencilState>( gfx, DepthStencilState::StencilMode::Mask ) );
}

void Box::DrawOutline( Graphics & gfx ) noexcept( !IS_DEBUG )
{
	isOutlineEnabled = true;
	for( auto& b : outlineBindables )
	{
		b->Bind( gfx );
	}
	gfx.DrawIndexed( QueryBindable<IndexBuffer>()->GetCount() );
	isOutlineEnabled = false;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	auto xm = DirectX::XMMatrixRotationRollPitchYaw( orientation.x, orientation.y, orientation.z ) *
		DirectX::XMMatrixTranslation( pos.x, pos.y, pos.z );
	if( isOutlineEnabled )
	{
		xm = DirectX::XMMatrixScaling( 1.015f, 1.015f, 1.015f ) * xm;
	}
	return xm;
}

void Box::SpawnControlWindow( Graphics & gfx, const char* name ) noexcept
{
	if( ImGui::Begin( name ) )
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
			QueryBindable<PixelConstantBuffer<BoxCBuff>>()->Update( gfx, cbuff );
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