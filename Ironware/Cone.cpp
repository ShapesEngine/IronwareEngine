/*!
 * \file Pyramid.cpp
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 *
 *
 */
#include "Cone.h"
#include "BindableBase.h"
#include "GraphicsExceptionMacros.h"
#include "Pyramid.h"

#include <array>

Cone::Cone( Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_int_distribution<>& tdist ) :
	ObjectBase( gfx, rng, adist, ddist, odist, rdist )
{
	namespace dx = DirectX;

	if( !IsStaticInitialized() )
	{
		auto pVertexShader = std::make_unique<VertexShader>( gfx, L"ColorBlendPhongVS.cso" );
		auto pVertexShaderBytecode = pVertexShader->GetBytecode();
		AddStaticBind( std::move( pVertexShader ) );

		AddStaticBind( std::make_unique<PixelShader>( gfx, L"ColorBlendPhongPS.cso" ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> descInputElement =
		{
			{ "Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "Color", 0u, DXGI_FORMAT_R8G8B8A8_UNORM, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
		};
		AddStaticBind( std::make_unique<InputLayout>( gfx, descInputElement, pVertexShaderBytecode ) );

		AddStaticBind( std::make_unique<PrimitiveTopology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

		struct PSMaterialConstant
		{
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[2];
		} colorConst;
		AddStaticBind( std::make_unique<PixelConstantBuffer<PSMaterialConstant>>( gfx, colorConst, 1u ) );
	}

	struct Vertex
	{
		dx::XMFLOAT3 pos;
		dx::XMFLOAT3 n;
		std::array<uint8_t, 3> colors;
	};

	const auto tesselation = tdist( rng );
	auto model = Pyramid::MakeTesselatedIndependentFaces<Vertex>( tesselation );
	model.SetNormalsIndependentFlat();
	// set vertex colors for mesh
	for( auto& v : model.vertices )
	{
		v.colors = { 10, 10, 255 };
	}
	for( size_t i = 0; i < tesselation; i++ )
	{
		model.vertices[i * 3].colors = { 255, 10, 10 };
	}

	// deform mesh linearly
	model.Transform( dx::XMMatrixScaling( 1.f, 1.f, 0.7f ) );

	AddBind( std::make_unique<VertexBuffer>( gfx, model.vertices ) );
	AddIndexBufferBind( std::make_unique<IndexBuffer>( gfx, model.indices ) );

	AddBind( std::make_unique<TransformCBuffer>( gfx, *this ) );
}