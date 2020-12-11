/*!
 * \class SolidSphere
 *
 * \brief 
 *
 * \author Yernar Aldabergenov
 * \date December 2020
 */
#include "SolidSphere.h"
#include "BindableBase.h"
#include "GraphicsExceptionMacros.h"
#include "Sphere.h"

SolidSphere::SolidSphere( Graphics& gfx, float radius )
{
	namespace dx = DirectX;

	if( !IsStaticInitialized() )
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};
		auto model = Sphere::Make<Vertex>();
		model.Transform( dx::XMMatrixScaling( radius, radius, radius ) );
		AddBind( std::make_unique<VertexBuffer>( gfx, model.vertices ) );
		AddIndexBuffer( std::make_unique<IndexBuffer>( gfx, model.indices ) );

		auto pVertexShader = std::make_unique<VertexShader>( gfx, L"SolidVS.cso" );
		auto pVertexShaderBytecode = pVertexShader->GetBytecode();
		AddStaticBind( std::move( pVertexShader ) );

		AddStaticBind( std::make_unique<PixelShader>( gfx, L"SolidPS.cso" ) );

		struct PSColorConstant
		{
			dx::XMFLOAT3 color = { 1.0f, 1.0f, 1.0f };
			float padding;
		} colorConst;
		AddStaticBind( std::make_unique<PixelConstantBuffer<PSColorConstant>>( gfx, colorConst ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> descInputElem =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		AddStaticBind( std::make_unique<InputLayout>( gfx, descInputElem, pVertexShaderBytecode ) );

		AddStaticBind( std::make_unique<Topology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformCBuffer>( gfx, *this ) );
}
