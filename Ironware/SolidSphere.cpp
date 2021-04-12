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
	AddBind( std::make_shared<VertexBuffer>( gfx, vbuff ) );
	AddBind( std::make_shared<IndexBuffer>( gfx, model.indices ) );

	auto pVertexShader = std::make_shared<VertexShader>( gfx, L"SolidVS.cso" );
	auto pVertexShaderBytecode = pVertexShader->GetBytecode();
	AddBind( std::move( pVertexShader ) );

	AddBind( std::make_shared<PixelShader>( gfx, L"SolidPS.cso" ) );

	pPixelCBuff = dynamic_cast<PixelConstantBuffer<dx::XMFLOAT3A>*>(
		AddBind( std::make_shared<PixelConstantBuffer<dx::XMFLOAT3A>>( gfx, color ) )
		);

	AddBind( std::make_shared<InputLayout>( gfx, vbuff.GetLayout(), pVertexShaderBytecode ) );

	AddBind( std::make_shared<PrimitiveTopology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	AddBind( std::make_shared<TransformCBuffer>( gfx, *this ) );
}

void SolidSphere::UpdateColor( Graphics& gfx, const DirectX::XMFLOAT3A& col ) noexcept
{
	color = col;
	pPixelCBuff->Update( gfx, color );
}