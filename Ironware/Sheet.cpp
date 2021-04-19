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
#include "Plane.h"

Sheet::Sheet( Graphics& gfx, float size )
{
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
		.Append( VertexLayout::ElementType::Texture2D )
	);
	for( auto& v : model.vertices )
	{
		vbuff.EmplaceBack( v.pos, v.n, v.tex );
	}
	const std::wstring& sheetTag = L"$sheet." + std::to_wstring( size );
	AddBind( VertexBuffer::Resolve( gfx, sheetTag, vbuff ) );
	AddBind( IndexBuffer::Resolve( gfx, sheetTag, model.indices ) );

	auto pVertexShader = VertexShader::Resolve( gfx, L"PhongDiffuseMapVS.cso" );
	auto pVertexShaderBytecode = pVertexShader->GetBytecode();
	AddBind( std::move( pVertexShader ) );

	AddBind( PixelShader::Resolve( gfx, L"PhongDiffuseMapPS.cso" ) );

	AddBind( Texture::Resolve( gfx, L"Images\\brickwall.jpg" ) );
	AddBind( Sampler::Resolve( gfx ) );

	struct SpecularCBuff
	{
		float intensity = 0.5f;
		float power = 50.f;
		alignas( 8 ) float padding;
	}spec;

	AddBind( PixelConstantBuffer<SpecularCBuff>::Resolve( gfx, spec, 1u ) );

	AddBind( InputLayout::Resolve( gfx, vbuff.GetLayout(), pVertexShaderBytecode ) );

	AddBind( PrimitiveTopology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	AddBind( std::make_shared<TransformCBuffer>( gfx, *this ) );
}

DirectX::XMMATRIX Sheet::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw( orientation.x, orientation.y, orientation.z ) *
		DirectX::XMMatrixTranslation( pos.x, pos.y, pos.z );
}
