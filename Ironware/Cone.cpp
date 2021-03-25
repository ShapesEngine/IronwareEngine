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

Cone::Cone( Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_int_distribution<>& tdist ) :
	zOffset( rdist( rng ) ),
	droll( ddist( rng ) ),
	dpitch( ddist( rng ) ),
	dyaw( ddist( rng ) ),
	dphi( odist( rng ) ),
	dtheta( odist( rng ) ),
	dchi( odist( rng ) ),
	chi( adist( rng ) ),
	theta( adist( rng ) ),
	phi( adist( rng ) )
{
	namespace dx = DirectX;

	if( !IsStaticInitialized() )
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
			std::array<uint8_t, 3> colors;
		};
		auto model = Pyramid::MakeTesselatedIndependentFaces<Vertex>( tdist( rng ) );
		model.SetNormalsIndependentFlat();
		// set vertex colors for mesh
		for( auto& v : model.vertices )
		{
			v.colors = { 40, 40, 255 };
		}
		model.vertices.front().colors = { 255,20,20 }; // very first vertex is the cone tip

		// deform mesh linearly
		model.Transform( dx::XMMatrixScaling( 1.f, 1.f, 0.7f ) );

		AddStaticBind( std::make_unique<VertexBuffer>( gfx, model.vertices ) );

		auto pVertexShader = std::make_unique<VertexShader>( gfx, L"ColorBlendPhongVS.cso" );
		auto pVertexShaderBytecode = pVertexShader->GetBytecode();
		AddStaticBind( std::move( pVertexShader ) );

		AddStaticBind( std::make_unique<PixelShader>( gfx, L"ColorBlendPhongPS.cso" ) );

		AddStaticIndexBufferBind( std::make_unique<IndexBuffer>( gfx, model.indices ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> descInputElement =
		{
			{ "Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "Color", 0u, DXGI_FORMAT_R8G8B8A8_UNORM, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
		};
		AddStaticBind( std::make_unique<InputLayout>( gfx, descInputElement, pVertexShaderBytecode ) );

		AddStaticBind( std::make_unique<PrimitiveTopology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformCBuffer>( gfx, *this ) );
}

void Cone::Update( float dt ) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Cone::GetTransformXM() const noexcept
{
	namespace dx = DirectX;

	return dx::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) *
		dx::XMMatrixTranslation( zOffset, 0.f, 0.f ) *
		dx::XMMatrixRotationRollPitchYaw( theta, phi, chi );
}
