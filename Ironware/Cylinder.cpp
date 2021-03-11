/*!
 * \file Cylinder.cpp
 * \date 2020/11/14 21:59
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief 
 *
 * TODO:
 *
 * \note
*/
#include "Cylinder.h"
#include "Prism.h"
#include "BindableBase.h"

Cylinder::Cylinder( Graphics& gfx, std::mt19937& rng, 
					std::uniform_real_distribution<float>& adist, 
					std::uniform_real_distribution<float>& ddist, 
					std::uniform_real_distribution<float>& odist, 
					std::uniform_real_distribution<float>& rdist, 
					std::uniform_real_distribution<float>& bdist, 
					std::uniform_int_distribution<int>& tdist ) :
	ObjectBase(gfx, rng, adist, ddist, odist, rdist)
{
	namespace dx = DirectX;

	if( !IsStaticInitialized() )
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
		};
		auto model = Prism::MakeTesselatedIndependentCapNormals<Vertex>( tdist( rng ) );

		AddStaticBind( std::make_unique<VertexBuffer>( gfx, model.vertices ) );

		auto pVertexShader = std::make_unique<VertexShader>( gfx, L"PhongLightVS.cso" );
		auto pVertexShaderBytecode = pVertexShader->GetBytecode();
		AddStaticBind( std::move( pVertexShader ) );

		AddStaticBind( std::make_unique<PixelShader>( gfx, L"IndexedPhongLightPS.cso" ) );

		AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx, model.indices ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> descInputElem =
		{
			{ "Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
		};
		AddStaticBind( std::make_unique<InputLayout>( gfx, descInputElem, pVertexShaderBytecode ) );

		AddStaticBind( std::make_unique<PrimitiveTopology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

		struct PSMaterialConstant
		{
			dx::XMFLOAT3A colors[6] = {
				{1.f, 0.f, 0.f},
				{0.f, 1.f, 0.f},
				{0.f, 0.f, 1.f},
				{1.f, 1.f, 0.f},
				{1.f, 0.f, 1.f},
				{0.f, 1.f, 1.f},
			};
			float specularIntensity = 0.6f;
			float specularPower = 30.f;
		} matConst;
		AddStaticBind( std::make_unique<PixelConstantBuffer<PSMaterialConstant>>( gfx, matConst, 1u ) );
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformCBuffer>( gfx, *this ) );
}
