/*!
 * \class Melon
 *
 * \ingroup DEV
 *
 * \brief 
 *
 * TODO:
 *
 * \note 
 *
 * \author Yernar Aldabergenov
 *
 * \version 1.0
 *
 * \date November 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
#include "Melon.h"
#include "BindableBase.h"
#include "GraphicsExceptionMacros.h"
#include "Sphere.h"


Melon::Melon( Graphics& gfx,
			  std::mt19937& rng,
			  std::uniform_real_distribution<float>& adist,
			  std::uniform_real_distribution<float>& ddist,
			  std::uniform_real_distribution<float>& odist,
			  std::uniform_real_distribution<float>& rdist,
			  std::uniform_int_distribution<int>& longdist,
			  std::uniform_int_distribution<int>& latdist ) :
	r( rdist( rng ) ),
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
		auto pVertexShader = std::make_unique<VertexShader>( gfx, L"ColorIndexVS.cso" );
		auto pVertexShaderBytecode = pVertexShader->GetBytecode();
		AddStaticBind( std::move( pVertexShader ) );

		AddStaticBind( std::make_unique<PixelShader>( gfx, L"ColorIndexPS.cso" ) );

		struct PixelShaderConstants
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[8];
		};
		const PixelShaderConstants constBuff =
		{
			{
				{ 1.f, 1.f, 1.f },
				{ 1.f, 0.f, 0.f },
				{ 0.f, 1.f, 0.f },
				{ 1.f, 1.f, 0.f },
				{ 0.f, 0.f, 1.f },
				{ 1.f, 0.f, 1.f },
				{ 0.f, 1.f, 1.f },
				{ 0.f, 0.f, 0.f },
			}
		};
		AddStaticBind( std::make_unique<PixelConstantBuffer<PixelShaderConstants>>( gfx, constBuff ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		AddStaticBind( std::make_unique<InputLayout>( gfx, ied, pVertexShaderBytecode ) );

		AddStaticBind( std::make_unique<Topology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}

	struct Vertex
	{
		dx::XMFLOAT3 pos;
	};
	auto model = Sphere::MakeTesselated<Vertex>( latdist( rng ), longdist( rng ) );
	// deform vertices of model by linear transformation
	model.Transform( dx::XMMatrixScaling( 1.f, 1.f, 1.2f ) );

	AddBind( std::make_unique<VertexBuffer>( gfx, model.vertices ) );

	AddIndexBuffer( std::make_unique<IndexBuffer>( gfx, model.indices ) );

	AddBind( std::make_unique<TransformCBuffer>( gfx, *this ) );
}

void Melon::Update( float dt ) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Melon::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) *
		dx::XMMatrixTranslation( r, 0.f, 0.f ) *
		dx::XMMatrixRotationRollPitchYaw( theta, phi, chi ) *
		dx::XMMatrixTranslation( 0.f, 0.f, 20.f );
}
