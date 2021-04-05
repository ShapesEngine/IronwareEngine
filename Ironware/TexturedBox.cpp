/*!
 * \file TexturedBox.cpp
 *
 * \author Yernar Aldabergenov
 * \date March 2021
 *
 *
 */
#include "TexturedBox.h"
#include "BindableCommon.h"
#include "GraphicsExceptionMacros.h"
#include "Cube.h"
#include "Texture.h"
#include "Sampler.h"
#include "Surface.h"

TexturedBox::TexturedBox( Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist ) :
	ObjectBase( gfx, rng, adist, ddist, odist, rdist )
{
	namespace dx = DirectX;

	if( !IsStaticInitialized() )
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
			dx::XMFLOAT2 tc;
		};
		auto model = Cube::MakeIndependentTextured<Vertex>();
		model.SetNormalsIndependentFlat();
		AddStaticBind( std::make_unique<VertexBuffer>( gfx, model.vertices ) );

		auto pVertShader = std::make_unique<VertexShader>( gfx, L"TexturedPhongVS.cso" );
		// save bytecode, as it will be needed in input layout
		auto pVertShaderBytecode = pVertShader->GetBytecode();
		AddStaticBind( std::move( pVertShader ) );

		AddStaticBind( std::make_unique<PixelShader>( gfx, L"TexturedPhongPS.cso" ) );

		AddStaticBind( std::make_unique<Texture>( gfx, Surface::FromFile( L"Images\\metro.jpg" ) ) );

		AddStaticBind( std::make_unique<Sampler>( gfx ) );

		AddStaticIndexBufferBind( std::make_unique<IndexBuffer>( gfx, model.indices ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> descInputElem =
		{
			{ "Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "TexCoord", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
		};
		AddStaticBind( std::make_unique<InputLayout>( gfx, descInputElem, pVertShaderBytecode ) );

		AddStaticBind( std::make_unique<PrimitiveTopology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformCBuffer>( gfx, *this ) );

	struct PSSpecularConst
	{
		float specularIntensity = 0.6f;
		float specularPower = 30.f;
		float padding[2];
	} specConst;
	AddBind( std::make_unique<PixelConstantBuffer<PSSpecularConst>>( gfx, specConst, 1u ) );

	// model deformation transform (per instance, not stored as bind)
	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling( 1.f, 1.f, bdist( rng ) )
	);
}

DirectX::XMMATRIX TexturedBox::GetTransformXM() const noexcept
{
	namespace dx = DirectX;

	return dx::XMLoadFloat3x3( &mt ) * ObjectBase::GetTransformXM();
}


