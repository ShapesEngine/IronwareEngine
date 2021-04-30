/*!
 * \file SolidSphere.cpp
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 *
 *
 */
#include "SolidSphere.h"
#include "BindableCommon.h"
#include "GraphicsExceptionMacros.h"
#include "Vertex.h"
#include "Sphere.h"

SolidSphere::SolidSphere( Graphics& gfx, float radius )
{
	namespace dx = DirectX;

	struct Vertex
	{
		dx::XMFLOAT3 pos;
	};
	auto model = Sphere::Make<Vertex>();
	model.Transform( dx::XMMatrixScaling( radius, radius, radius ) );
	VertexByteBuffer vbuff(
		VertexLayout{}
		.Append( VertexLayout::ElementType::Position3D )
	);
	for( auto& v : model.vertices )
	{
		vbuff.EmplaceBack( v.pos );
	}
	const std::wstring& sphereTag = L"$sphere." + std::to_wstring( radius );
	AddBind( VertexBuffer::Resolve( gfx, sphereTag, vbuff ) );
	AddBind( IndexBuffer::Resolve( gfx, sphereTag, model.indices ) );

	auto pVertexShader = VertexShader::Resolve( gfx, L"SolidVS.cso" );
	auto pVertexShaderBytecode = pVertexShader->GetBytecode();
	AddBind( std::move( pVertexShader ) );

	AddBind( PixelShader::Resolve( gfx, L"SolidPS.cso" ) );

	AddBind( PixelConstantBuffer<dx::XMFLOAT3A>::Resolve( gfx, color, 1u ) );

	AddBind( InputLayout::Resolve( gfx, vbuff.GetLayout(), pVertexShaderBytecode ) );

	AddBind( PrimitiveTopology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	AddBind( std::make_shared<TransformCBuffer>( gfx, *this ) );

	AddBind( BlendState::Resolve( gfx, false ) );

	AddBind( RasterizerState::Resolve( gfx, false ) );

	AddBind( std::make_shared<DepthStencilState>( gfx, DepthStencilState::StencilMode::Off ) );
}

void SolidSphere::UpdateColor( Graphics& gfx, const DirectX::XMFLOAT3A& col ) noexcept
{
	color = col;
	QueryBindable<PixelConstantBuffer<DirectX::XMFLOAT3A>>()->Update( gfx, color );
}