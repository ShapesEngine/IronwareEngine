/*!
 * \file CameraIndicator.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "CameraIndicator.h"
#include "BindableCommon.h"
#include "Vertex.h"
#include "Sphere.h"
#include "DepthStencilState.h"
#include "IronChannels.h"

CameraIndicator::CameraIndicator( Graphics& gfx )
{
	const auto geometryTag = L"$cam";
	VertexLayout layout;
	layout.Append( VertexLayout::ElementType::Position3D );
	VertexByteBuffer vertices{ std::move( layout ) };
	{
		const float x = 4.0f / 3.0f * 0.75f;
		const float y = 1.0f * 0.75f;
		const float z = -2.0f;
		const float thalf = x * 0.5f;
		const float tspace = y * 0.2f;
		vertices.EmplaceBack( dx::XMFLOAT3{ -x,y,0.0f } );
		vertices.EmplaceBack( dx::XMFLOAT3{ x,y,0.0f } );
		vertices.EmplaceBack( dx::XMFLOAT3{ x,-y,0.0f } );
		vertices.EmplaceBack( dx::XMFLOAT3{ -x,-y,0.0f } );
		vertices.EmplaceBack( dx::XMFLOAT3{ 0.0f,0.0f,z } );
		vertices.EmplaceBack( dx::XMFLOAT3{ -thalf,y + tspace,0.0f } );
		vertices.EmplaceBack( dx::XMFLOAT3{ thalf,y + tspace,0.0f } );
		vertices.EmplaceBack( dx::XMFLOAT3{ 0.0f,y + tspace + thalf,0.0f } );
	}
	std::vector<uint16_t> indices;
	{
		indices.push_back( 0u );
		indices.push_back( 1u );
		indices.push_back( 1u );
		indices.push_back( 2u );
		indices.push_back( 2u );
		indices.push_back( 3u );
		indices.push_back( 3u );
		indices.push_back( 0u );
		indices.push_back( 0u );
		indices.push_back( 4u );
		indices.push_back( 1u );
		indices.push_back( 4u );
		indices.push_back( 2u );
		indices.push_back( 4u );
		indices.push_back( 3u );
		indices.push_back( 4u );
		indices.push_back( 5u );
		indices.push_back( 6u );
		indices.push_back( 6u );
		indices.push_back( 7u );
		indices.push_back( 7u );
		indices.push_back( 5u );
	}

	pVertices = VertexBuffer::Resolve( gfx, geometryTag, vertices );
	pIndices = IndexBuffer::Resolve( gfx, geometryTag, indices );
	pTopology = PrimitiveTopology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_LINELIST );

	{
		RenderTechnique line{ IR_CH::main };
		RenderStep only( "lambertian" );

		auto pvs = VertexShader::Resolve( gfx, L"Solid_VS.cso" );
		only.AddBindable( InputLayout::Resolve( gfx, vertices.GetLayout(), *pvs ) );
		only.AddBindable( std::move( pvs ) );

		only.AddBindable( PixelShader::Resolve( gfx, L"Solid_PS.cso" ) );

		struct PSColorConstant
		{
			dx::XMFLOAT3A color = { 0.94f, 0.45f, 0.15f };
		} colorConst;
		only.AddBindable( PixelConstantBuffer<PSColorConstant>::Resolve( gfx, colorConst, 1u ) );

		only.AddBindable( std::make_shared<TransformCBuffer>( gfx ) );

		only.AddBindable( RasterizerState::Resolve( gfx, false ) );

		line.AddStep( std::move( only ) );
		AddTechnique( std::move( line ) );
	}
}

DirectX::XMMATRIX CameraIndicator::GetTransformXM() const noexcept
{
	return dx::XMMatrixRotationRollPitchYawFromVector( dx::XMLoadFloat3( &rot ) ) *
		dx::XMMatrixTranslationFromVector( dx::XMLoadFloat3( &pos ) );
}
