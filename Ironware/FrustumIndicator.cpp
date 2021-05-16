/*!
 * \file FrustumIndicator.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "FrustumIndicator.h"
#include "BindableCommon.h"
#include "Vertex.h"
#include "Sphere.h"
#include "DepthStencilState.h"

FrustumIndicator::FrustumIndicator( Graphics & gfx, float width, float height, float nearZ, float farZ )
{
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
		indices.push_back( 4u );
		indices.push_back( 5u );
		indices.push_back( 5u );
		indices.push_back( 6u );
		indices.push_back( 6u );
		indices.push_back( 7u );
		indices.push_back( 7u );
		indices.push_back( 4u );
		indices.push_back( 0u );
		indices.push_back( 4u );
		indices.push_back( 1u );
		indices.push_back( 5u );
		indices.push_back( 2u );
		indices.push_back( 6u );
		indices.push_back( 3u );
		indices.push_back( 7u );
	}

	SetVertices( gfx, width, height, nearZ, farZ );
	pIndices = IndexBuffer::Resolve( gfx, L"$frustum", indices );
	pTopology = PrimitiveTopology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_LINELIST );

	{
		RenderTechnique line;
		RenderStep only( "lambertian" );

		auto pvs = VertexShader::Resolve( gfx, L"Solid_VS.cso" );
		only.AddBindable( InputLayout::Resolve( gfx, pVertices->GetLayout(), *pvs ) );
		only.AddBindable( std::move( pvs ) );

		only.AddBindable( PixelShader::Resolve( gfx, L"Solid_PS.cso" ) );

		struct PSColorConstant
		{
			dx::XMFLOAT3 color = { 0.6f,0.2f,0.2f };
			float padding;
		} colorConst;
		only.AddBindable( PixelConstantBuffer<PSColorConstant>::Resolve( gfx, colorConst, 1u ) );

		only.AddBindable( std::make_shared<TransformCBuffer>( gfx ) );

		only.AddBindable( RasterizerState::Resolve( gfx, false ) );

		line.AddStep( std::move( only ) );
		AddTechnique( std::move( line ) );
	}
}

void FrustumIndicator::SetVertices( Graphics & gfx, float width, float height, float nearZ, float farZ )
{
	VertexLayout layout;
	layout.Append( VertexLayout::ElementType::Position3D );
	VertexByteBuffer vertices{ std::move( layout ) };
	{
		const float zRatio = farZ / nearZ;
		const float nearX = width / 2.0f;
		const float nearY = height / 2.0f;
		const float farX = nearX * zRatio;
		const float farY = nearY * zRatio;
		vertices.EmplaceBack( dx::XMFLOAT3{ -nearX,nearY,nearZ } );
		vertices.EmplaceBack( dx::XMFLOAT3{ nearX,nearY,nearZ } );
		vertices.EmplaceBack( dx::XMFLOAT3{ nearX,-nearY,nearZ } );
		vertices.EmplaceBack( dx::XMFLOAT3{ -nearX,-nearY,nearZ } );
		vertices.EmplaceBack( dx::XMFLOAT3{ -farX,farY,farZ } );
		vertices.EmplaceBack( dx::XMFLOAT3{ farX,farY,farZ } );
		vertices.EmplaceBack( dx::XMFLOAT3{ farX,-farY,farZ } );
		vertices.EmplaceBack( dx::XMFLOAT3{ -farX,-farY,farZ } );
	}
	pVertices = std::make_shared<VertexBuffer>( gfx, vertices );
}

DirectX::XMMATRIX FrustumIndicator::GetTransformXM() const noexcept
{
	return dx::XMMatrixRotationRollPitchYawFromVector( dx::XMLoadFloat3( &rot ) ) *
		dx::XMMatrixTranslationFromVector( dx::XMLoadFloat3( &pos ) );
}