/*!
 * \class Pyramid
 *
 * \ingroup DEV
 *
 * \brief A Pyramid child class will control(partly) the graphics pipeline and draw the box
 *
 * TODO:
 *
 * \note The class contains variables that are needed for translation, rotation, etc.
 *
 * \author Yernar Aldabergenov
 *
 * \version 1.0
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
#include "Pyramid.h"
#include "BindableBase.h"
#include "GraphicsExceptionMacros.h"
#include "Cone.h"

Pyramid::Pyramid( Graphics& gfx,
				  std::mt19937& rng,
				  std::uniform_real_distribution<float>& adist,
				  std::uniform_real_distribution<float>& ddist,
				  std::uniform_real_distribution<float>& odist,
				  std::uniform_real_distribution<float>& rdist ) :
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
			struct
			{
				uint8_t zOffset;
				uint8_t g;
				uint8_t b;
				uint8_t a;
			} color;
		};
		auto model = Cone::MakeTesselated<Vertex>( 4 );
		// set vertex colors for mesh
		model.vertices[0].color = { 255, 255, 0 };
		model.vertices[1].color = { 255, 255, 0 };
		model.vertices[2].color = { 255, 255, 0 };
		model.vertices[3].color = { 255, 255, 0 };
		model.vertices[4].color = { 255, 255, 80 };
		model.vertices[5].color = { 255, 10, 0 };
		// deform mesh linearly
		model.Transform( dx::XMMatrixScaling( 1.f, 1.f, 0.7f ) );

		AddStaticBind( std::make_unique<VertexBuffer>( gfx, model.vertices ) );

		auto pVertexShader = std::make_unique<VertexShader>( gfx, L"ColorBlendVS.cso" );
		auto pVertexShaderBytecode = pVertexShader->GetBytecode();
		AddStaticBind( std::move( pVertexShader ) );

		AddStaticBind( std::make_unique<PixelShader>( gfx, L"ColorBlendPS.cso" ) );

		AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx, model.indices ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> descInputElement =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		AddStaticBind( std::make_unique<InputLayout>( gfx, descInputElement, pVertexShaderBytecode ) );

		AddStaticBind( std::make_unique<Topology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformCBuffer>( gfx, *this ) );
}

void Pyramid::Update( float dt ) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Pyramid::GetTransformXM() const noexcept
{
	namespace dx = DirectX;

	return dx::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) *
		dx::XMMatrixTranslation( zOffset, 0.f, 0.f ) *
		dx::XMMatrixRotationRollPitchYaw( theta, phi, chi );
}
