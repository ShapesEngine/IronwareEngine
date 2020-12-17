/*!
 * \class Box
 *
 * \brief A Box child class will control(partly) the graphics pipeline and draw the box
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
#include "Box.h"
#include "BindableBase.h"
#include "GraphicsExceptionMacros.h"
#include "Cube.h"

Box::Box( Graphics& gfx, std::mt19937& rng, 
          std::uniform_real_distribution<float>& adist, 
          std::uniform_real_distribution<float>& ddist, 
          std::uniform_real_distribution<float>& odist, 
          std::uniform_real_distribution<float>& rdist,
		  std::uniform_real_distribution<float>& bdist,
		  DirectX::XMFLOAT3 materialColor ) :
	ObjectBase( gfx, rng, adist, ddist, odist, rdist )
{
	namespace dx = DirectX;

	if( !IsStaticInitialized() )
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
		};
		auto model = Cube::MakeIndependent<Vertex>();
		model.SetNormalsIndependentFlat();
		AddStaticBind( std::make_unique<VertexBuffer>( gfx, model.vertices ) );

		auto pVertShader = std::make_unique<VertexShader>( gfx, L"PhongLightVS.cso" );
		// save bytecode, as it will be needed in input layout
		auto pVertShaderBytecode = pVertShader->GetBytecode();
		AddStaticBind( std::move( pVertShader ) );

		AddStaticBind( std::make_unique<PixelShader>( gfx, L"PhongLightPS.cso" ) );

		AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx, model.indices ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> descInputElem =
		{
			{ "Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		AddStaticBind( std::make_unique<InputLayout>( gfx, descInputElem, pVertShaderBytecode ) );

		AddStaticBind( std::make_unique<Topology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformCBuffer>( gfx, *this ) );

	struct PSMaterialConstant
	{
		dx::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.f;
		float padding[3];
	} colorConst;
	colorConst.color = materialColor;
	AddBind( std::make_unique<PixelConstantBuffer<PSMaterialConstant>>( gfx, colorConst, 1u ) );

	// model deformation transform (per instance, not stored as bind)
	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling( 1.f,  1.f,  bdist( rng ) )
	);
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	namespace dx = DirectX;

	return dx::XMLoadFloat3x3( &mt ) * ObjectBase::GetTransformXM();
}
