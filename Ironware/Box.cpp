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
#include "Cube.h"
#include "DynamicConstantBuffer.h"
#include "RenderTechnique.h"
#include "TechniqueProbe.h"

#include <imgui/imgui.h>

Box::Box( Graphics & gfx, float size )
{
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
		RenderTechnique lambertian{ L"Lambertian" };
		{
			RenderStep lambertianStep{ 0ull };
			auto pVertexShader = VertexShader::Resolve( gfx, L"PhongDiffuseMapVS.cso" );
			auto pVertexShaderBytecode = pVertexShader->GetBytecode();
			lambertianStep.AddBindable( std::move( pVertexShader ) );

			lambertianStep.AddBindable( PixelShader::Resolve( gfx, L"PhongDiffuseMapPS.cso" ) );

			lambertianStep.AddBindable( Texture::Resolve( gfx, L"Images\\brickwall.jpg" ) );
			/*lambertian.AddBindable( Texture::Resolve( gfx, L"Images\\brickwall_normal.jpg", 1u ) );*/
			lambertianStep.AddBindable( Sampler::Resolve( gfx ) );

			RawLayout lay;
			lay.Add<Float>( "specularIntensity" );
			lay.Add<Float>( "specularPower" );
			auto buf = Buffer( std::move( lay ) );
			buf["specularIntensity"] = 0.1f;
			buf["specularPower"] = 20.0f;
			lambertianStep.AddBindable( std::make_shared<CachingPixelConstantBufferEx>( gfx, buf, 1u ) );

			lambertianStep.AddBindable( InputLayout::Resolve( gfx, vbuff.GetLayout(), pVertexShaderBytecode ) );

			lambertianStep.AddBindable( std::make_shared<TransformCBuffer>( gfx ) );

			lambertian.AddStep( std::move( lambertianStep ) );
		}
		AddTechnique( std::move( lambertian ) );
	}

	{
		RenderTechnique outlineRT{ L"Outline" };
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

			RawLayout lay;
			lay.Add<Float4>( "color" );
			auto buf = Buffer( std::move( lay ) );
			buf["color"] = DirectX::XMFLOAT4{ 1.0f, 1.f, 0.5f, 1.0f };
			outline.AddBindable( std::make_shared<CachingPixelConstantBufferEx>( gfx, buf, 1u ) );

			outline.AddBindable( InputLayout::Resolve( gfx, vbuff.GetLayout(), pVertexShaderBytecode ) );

			class TransformCBufferScaling : public TransformCBuffer
			{
			public:
				TransformCBufferScaling( Graphics& gfx, float scale = 1.315 ) :
					TransformCBuffer( gfx ),
					buf( MakeLayout() )
				{
					buf["scale"] = scale;
				}

				void Accept( TechniqueProbe& probe ) override
				{
					probe.VisitBuffer( buf );
				}

				void Bind( Graphics& gfx ) noexcept override
				{
					const auto scale = dx::XMMatrixScaling( 1.015f, 1.015f, 1.015f );
					auto tf = GetTransform( gfx );
					tf.modelView = tf.modelView * scale;
					tf.modelViewProj = tf.modelViewProj * scale;
					UpdateBind( gfx, tf );
				}

			private:
				static RawLayout MakeLayout()
				{
					RawLayout layout;
					layout.Add<Float>( "scale" );
					return layout;
				}
			private:
				Buffer buf;
			};

			outline.AddBindable( std::make_shared<TransformCBufferScaling>( gfx ) );

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

		class Probe : public TechniqueProbe
		{
		public:
			void OnSetTechnique() override
			{
				using namespace std::string_literals;
				ImGui::TextColored( { 0.4f, 1.0f, 0.6f, 1.0f }, to_narrow( pTech->GetName() ).c_str() );
				bool active = pTech->IsActive();
				ImGui::Checkbox( ( "Tech Active##"s + to_narrow( pTech->GetName() ) ).c_str(), &active );
				pTech->SetActive( active );
			}
			bool VisitBuffer( Buffer& buf ) override
			{
				namespace dx = DirectX;
				float dirty = false;
				const auto dcheck = [&dirty]( bool changed ) {dirty = dirty || changed; };

				if( auto v = buf["scale"]; v.Exists() )
				{
					dcheck( ImGui::SliderFloat( "Scale", &v, 1.0f, 2.0f, "%.3f", 3.5f ) );
				}
				if( auto v = buf["color"]; v.Exists() )
				{
					dcheck( ImGui::ColorPicker4( "Color", reinterpret_cast<float*>( &static_cast<dx::XMFLOAT4&>( v ) ) ) );
				}
				if( auto v = buf["specularIntensity"]; v.Exists() )
				{
					dcheck( ImGui::SliderFloat( "Specular Intensity", &v, 0.0f, 1.0f ) );
				}
				if( auto v = buf["specularPower"]; v.Exists() )
				{
					dcheck( ImGui::SliderFloat( "Glossiness", &v, 1.0f, 100.0f, "%.1f", 1.5f ) );
				}
				return dirty;
			}
		};

		static Probe probe;
		Accept( probe );
	}
	ImGui::End();
}