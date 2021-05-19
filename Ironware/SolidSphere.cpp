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
#include "IronChannels.h"

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
	const std::wstring& sphereTag{ L"$sphere." + std::to_wstring( radius ) };

	for( auto& v : model.vertices )
	{
		vbuff.EmplaceBack( v.pos );
	}
	pVertices = VertexBuffer::Resolve( gfx, sphereTag, vbuff );
	pIndices = IndexBuffer::Resolve( gfx, sphereTag, model.indices );
	pTopology = PrimitiveTopology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	{
		RenderTechnique techq{ IR_CH::main };
		RenderStep only{ "lambertian" };

		auto pVertexShader = VertexShader::Resolve( gfx, L"Solid_VS.cso" );

		only.AddBindable( InputLayout::Resolve( gfx, vbuff.GetLayout(), *pVertexShader ) );

		only.AddBindable( std::move( pVertexShader ) );

		only.AddBindable( PixelShader::Resolve( gfx, L"Solid_PS.cso" ) );

		only.AddBindable( PixelConstantBuffer<dx::XMFLOAT3A>::Resolve( gfx, color, 1u ) );

		only.AddBindable( std::make_shared<TransformCBuffer>( gfx ) );

		only.AddBindable( BlendState::Resolve( gfx, false ) );

		only.AddBindable( RasterizerState::Resolve( gfx, false ) );

		techq.AddStep( std::move( only ) );
		AddTechnique( std::move( techq ) );
	}
}

void SolidSphere::UpdateColor( Graphics& gfx, const DirectX::XMFLOAT3A& col ) noexcept
{
	color = col;
	//QueryBindable<PixelConstantBuffer<DirectX::XMFLOAT3A>>()->Update( gfx, color );
}