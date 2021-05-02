/*!
 * \file Box.cpp
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#include "Box.h"
#include "Vertex.h"
#define IR_INCLUDE_TEXTURE
#include "BindableCommon.h"
#include "RenderStep.h"
#include <imgui/imgui.h>
#include "Cube.h"

Box::Box( Graphics & gfx, float size )
{
	Reset();

	namespace dx = DirectX;

	struct Vertex
	{
		dx::XMFLOAT3 pos;
		dx::XMFLOAT3 n;
		dx::XMFLOAT2 tex;
	};

	auto model = Cube::MakeIndependentTextured<Vertex>();
	model.Transform( DirectX::XMMatrixScaling( size, size, size ) );
	model.SetNormalsIndependentFlat();

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
	const std::wstring& boxTag = L"$box." + std::to_wstring( size );
	pVertices = VertexBuffer::Resolve( gfx, boxTag, vbuff );
	pIndices = IndexBuffer::Resolve( gfx, boxTag, model.indices );
	pTopology = PrimitiveTopology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	{
		RenderTechnique box;
		{
			RenderStep lambertian{ 0ull };
			auto pVertexShader = VertexShader::Resolve( gfx, L"PhongDiffuseMapVS.cso" );
			auto pVertexShaderBytecode = pVertexShader->GetBytecode();
			lambertian.AddBindable( std::move( pVertexShader ) );

			lambertian.AddBindable( PixelShader::Resolve( gfx, L"PhongDiffuseMapPS.cso" ) );

			lambertian.AddBindable( Texture::Resolve( gfx, L"Images\\brickwall.jpg" ) );
			/*lambertian.AddBindable( Texture::Resolve( gfx, L"Images\\brickwall_normal.jpg", 1u ) );*/
			lambertian.AddBindable( Sampler::Resolve( gfx ) );

			lambertian.AddBindable( PixelConstantBuffer<BoxCBuff>::Resolve( gfx, cbuff, 1u ) );

			lambertian.AddBindable( InputLayout::Resolve( gfx, vbuff.GetLayout(), pVertexShaderBytecode ) );

			lambertian.AddBindable( std::make_shared<TransformCBuffer>( gfx ) );

			box.AddStep( std::move( lambertian ) );
		}
		AddTechnique( std::move( box ) );
	}

	{
		RenderTechnique outlineRT;
		{
			RenderStep mask{ 1ull };
			auto pVertexShader = VertexShader::Resolve( gfx, L"SolidVS.cso" );
			auto pVertexShaderBytecode = pVertexShader->GetBytecode();
			mask.AddBindable( std::move( pVertexShader ) );

			mask.AddBindable( InputLayout::Resolve( gfx, vbuff.GetLayout(), pVertexShaderBytecode ) );

			mask.AddBindable( std::make_shared<TransformCBuffer>( gfx ) );

			outlineRT.AddStep( std::move( mask ) );
		}

		{
			RenderStep outline{ 2ull };
			auto pVertexShader = VertexShader::Resolve( gfx, L"SolidVS.cso" );
			auto pVertexShaderBytecode = pVertexShader->GetBytecode();
			outline.AddBindable( std::move( pVertexShader ) );

			outline.AddBindable( PixelShader::Resolve( gfx, L"SolidPS.cso" ) );

			outline.AddBindable( InputLayout::Resolve( gfx, vbuff.GetLayout(), pVertexShaderBytecode ) );

			class TransformCBufferScaled : public TransformCBuffer
			{
			public:
				using TransformCBuffer::TransformCBuffer;
				void Bind( Graphics& gfx ) noexcept override
				{
					const auto scale = dx::XMMatrixScaling( 1.04f, 1.04f, 1.04f );
					auto tf = GetTransform( gfx );
					tf.modelView = tf.modelView * scale;
					tf.modelViewProj = tf.modelViewProj * scale;
					UpdateBind( gfx, tf );
				}
			};

			outline.AddBindable( std::make_shared<TransformCBufferScaled>( gfx ) );

			outlineRT.AddStep( std::move( outline ) );
		}
		AddTechnique( std::move( outlineRT ) );
	}
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw( orientation.x, orientation.y, orientation.z ) *
		DirectX::XMMatrixTranslation( pos.x, pos.y, pos.z );
}

void Box::SpawnControlWindow( Graphics & gfx, const char* name ) noexcept
{
	if( ImGui::Begin( name ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X", &pos.x, -60.f, 60.f, "%.1f" );
		ImGui::SliderFloat( "Y", &pos.y, -60.f, 60.f, "%.1f" );
		ImGui::SliderFloat( "Z", &pos.z, -60.f, 60.f, "%.1f" );

		ImGui::Text( "Orientation" );
		ImGui::SliderAngle( "Pitch", &orientation.x, -180.0f, 180.0f );
		ImGui::SliderAngle( "Yaw", &orientation.y, -180.0f, 180.0f );
		ImGui::SliderAngle( "Roll", &orientation.z, -180.0f, 180.0f );

		ImGui::Text( "Properties" );
		const bool bi = ImGui::SliderFloat( "Specular Intensity", &cbuff.specularIntensity, 0.01f, 1.f, "%.2f", 2 );
		const bool bp = ImGui::SliderFloat( "Specular Power", &cbuff.specularPower, 10.f, 50.f, "%.f", 2 );
		const bool bnm = ImGui::Checkbox( "Enable Normal Mapping", reinterpret_cast<bool*>( &cbuff.isNormalMappingEnabled ) );

		if( bi || bp || bnm )
		{
			//QueryBindable<PixelConstantBuffer<BoxCBuff>>()->Update( gfx, cbuff );
		}

		if( ImGui::Button( "Reset" ) )
		{
			Reset();
		}
	}
	ImGui::End();
}

void Box::Reset() noexcept
{
	pos = { 10.f, 10.f, 0.f };
	orientation = { 0.f, 0.f, 0.f };
	cbuff.specularIntensity = 0.1f;
	cbuff.specularPower = 22.f;
	cbuff.isNormalMappingEnabled = TRUE;
}