/*!
 * \file Box.cpp
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 *
 *
 */
#include "Box.h"
#include "BindableCommon.h"
#include "GraphicsExceptionMacros.h"
#include "Cube.h"

#include <imgui/imgui.h>

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

		auto pVertShader = std::make_unique<VertexShader>( gfx, L"PhongVS.cso" );
		// save bytecode, as it will be needed in input layout
		auto pVertShaderBytecode = pVertShader->GetBytecode();
		AddStaticBind( std::move( pVertShader ) );

		AddStaticBind( std::make_unique<PixelShader>( gfx, L"PhongPS.cso" ) );

		AddStaticIndexBufferBind( std::make_unique<IndexBuffer>( gfx, model.indices ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> descInputElem =
		{
			{ "Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
			{ "Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
		};
		AddStaticBind( std::make_unique<InputLayout>( gfx, descInputElem, pVertShaderBytecode ) );

		AddStaticBind( std::make_unique<PrimitiveTopology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformCBuffer>( gfx, *this ) );

	materialConstants.color = materialColor;
	AddBind( std::make_unique<MaterialCBuf>( gfx, materialConstants, 1u ) );

	// model deformation transform (per instance, not stored as bind)
	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling( 1.f, 1.f, bdist( rng ) )
	);
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	namespace dx = DirectX;

	return dx::XMLoadFloat3x3( &mt ) * ObjectBase::GetTransformXM();
}

bool Box::SpawnControlWindow( uint32_t id, Graphics& gfx ) IFNOEXCEPT
{
	bool dirty = false;
	bool isOpen = true;
	if( ImGui::Begin( ( "Box: " + std::to_string( id ) ).c_str(), &isOpen ) )
	{
		ImGui::Text( "Material Properties" );
		const bool mc = ImGui::ColorEdit3( "Material Color", &materialConstants.color.x );
		const bool si = ImGui::SliderFloat( "Specular Intensity", &materialConstants.specularIntensity, 0.f, 1.f );
		const bool sp = ImGui::SliderFloat( "Specular Power", &materialConstants.specularPower, 0.f, 100.f, "%.f" );
		dirty = mc || si || sp;

		// it's unnecessary to check for dirty when controlling position and roatation,
		// as it will be updated by transformcbuf
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "Z Offset", &zOffset, 0.f, 80.f, "%.1f" );
		ImGui::SliderAngle( "X Rotation", &alpha, -180.f, 180.f );
		ImGui::SliderAngle( "Y Rotation", &beta, -180.f, 180.f );
		ImGui::Text( "Orientation" );
		ImGui::SliderAngle( "Roll", &roll, -180.f, 180.f );
		ImGui::SliderAngle( "Pitch", &pitch, -180.f, 180.f );
		ImGui::SliderAngle( "Yaw", &yaw, -180.f, 180.f );
	}

	ImGui::End();

	if( dirty )
	{
		SyncMaterials( gfx );
	}

	return isOpen;
}

void Box::SyncMaterials( Graphics& gfx )
{
	const auto pMatCbuf = QueryBindable<MaterialCBuf>();
	assert( pMatCbuf );
	pMatCbuf->Update( gfx, materialConstants );
}
