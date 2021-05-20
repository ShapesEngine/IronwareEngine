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
#include "IronChannels.h"

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

	VertexLayout layout;
	layout
		.Append( VertexLayout::ElementType::Position3D )
		.Append( VertexLayout::ElementType::Normal )
		.Append( VertexLayout::ElementType::Texture2D );

	VertexByteBuffer vbuff( layout );
	for( auto& v : model.vertices )
	{
		vbuff.EmplaceBack( v.pos, v.n, v.tex );
	}
	const std::wstring& boxTag = L"$box." + std::to_wstring( size );
	pVertices = VertexBuffer::Resolve( gfx, boxTag, vbuff );
	pIndices = IndexBuffer::Resolve( gfx, boxTag, model.indices );
	pTopology = PrimitiveTopology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	auto tcb = std::make_shared<TransformCBuffer>( gfx );

	{
		RenderTechnique shade( L"Shade", IR_CH::main );
		{
			RenderStep only( "lambertian" );

			only.AddBindable( Texture::Resolve( gfx, L"Images\\brickwall.jpg" ) );
			only.AddBindable( Sampler::Resolve( gfx ) );

			auto pvs = VertexShader::Resolve( gfx, L"Shadow_VS.cso" );

			only.AddBindable( InputLayout::Resolve( gfx, layout, *pvs ) );

			only.AddBindable( std::move( pvs ) );

			only.AddBindable( PixelShader::Resolve( gfx, L"Shadow_PS.cso" ) );

			RawLayout lay;
			lay.Add<Float3>( "specularColor" );
			lay.Add<Float>( "specularWeight" );
			lay.Add<Float>( "specularGloss" );
			auto buf = Buffer( std::move( lay ) );
			buf["specularColor"] = dx::XMFLOAT3{ 1.f,1.f,1.f };
			buf["specularWeight"] = 0.1f;
			buf["specularGloss"] = 20.f;
			only.AddBindable( std::make_shared<CachingPixelConstantBufferEx>( gfx, buf, 1u ) );


			only.AddBindable( RasterizerState::Resolve( gfx, false ) );

			only.AddBindable( tcb );

			shade.AddStep( std::move( only ) );
		}
		AddTechnique( std::move( shade ) );
	}
	{
		RenderTechnique outline( L"Outline", IR_CH::main );
		{
			RenderStep mask( "outlineMask" );

			// TODO: better sub-layout generation tech for future consideration maybe
			mask.AddBindable( InputLayout::Resolve( gfx, layout, *VertexShader::Resolve( gfx, L"Solid_VS.cso" ) ) );

			mask.AddBindable( std::move( tcb ) );

			// TODO: might need to specify rasterizer when doubled-sided models start being used

			outline.AddStep( std::move( mask ) );
		}
		{
			RenderStep draw( "outlineDraw" );

			RawLayout lay;
			lay.Add<Float4>( "color" );
			auto buf = Buffer( std::move( lay ) );
			buf["color"] = DirectX::XMFLOAT4{ 1.f, 1.f, 0.5f, 1.f };
			draw.AddBindable( std::make_shared<CachingPixelConstantBufferEx>( gfx, buf, 1u ) );

			// TODO: better sub-layout generation tech for future consideration maybe
			draw.AddBindable( InputLayout::Resolve( gfx, layout, *VertexShader::Resolve( gfx, L"Solid_VS.cso" ) ) );

			draw.AddBindable( std::make_shared<TransformCBuffer>( gfx ) );

			// TODO: might need to specify rasterizer when doubled-sided models start being used

			outline.AddStep( std::move( draw ) );
		}
		AddTechnique( std::move( outline ) );
	}
	// shadow map technique
	{
		RenderTechnique map{ L"ShadowMap", IR_CH::shadow, true };
		{
			RenderStep draw( "shadowMap" );

			draw.AddBindable( InputLayout::Resolve( gfx, layout, *VertexShader::Resolve( gfx, L"Solid_VS.cso" ) ) );

			draw.AddBindable( std::make_shared<TransformCBuffer>( gfx ) );

			map.AddStep( std::move( draw ) );
		}
		AddTechnique( std::move( map ) );
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
		ImGui::SliderAngle( "Pitch", &orientation.x, -180.f, 180.f );
		ImGui::SliderAngle( "Yaw", &orientation.y, -180.f, 180.f );
		ImGui::SliderAngle( "Roll", &orientation.z, -180.f, 180.f );

		class Probe : public TechniqueProbe
		{
		public:
			void OnSetTechnique() override
			{
				using namespace std::string_literals;
				ImGui::TextColored( { 0.4f, 1.f, 0.6f, 1.f }, to_narrow( pTech->GetName() ).c_str() );
				bool active = pTech->IsActive();
				ImGui::Checkbox( ( "Tech Active##"s + std::to_string( techIdx ) ).c_str(), &active );
				pTech->SetActive( active );
			}
			bool OnVisitBuffer( Buffer& buf ) override
			{
				namespace dx = DirectX;
				float dirty = false;
				const auto dcheck = [&dirty]( bool changed ) {dirty = dirty || changed; };
				auto tag = [tagScratch = std::string{}, tagString = "##" + std::to_string( bufIdx )]
				( const char* label ) mutable
				{
					tagScratch = label + tagString;
					return tagScratch.c_str();
				};

				if( auto v = buf["scale"]; v.Exists() )
				{
					dcheck( ImGui::SliderFloat( tag( "Scale" ), &v, 1.f, 2.f, "%.3f", 3.5f ) );
				}
				if( auto v = buf["color"]; v.Exists() )
				{
					dcheck( ImGui::ColorPicker4( tag( "Color" ), reinterpret_cast<float*>( &static_cast<dx::XMFLOAT4&>( v ) ) ) );
				}
				if( auto v = buf["specularIntensity"]; v.Exists() )
				{
					dcheck( ImGui::SliderFloat( tag( "Specular Intensity" ), &v, 0.f, 1.f ) );
				}
				if( auto v = buf["specularPower"]; v.Exists() )
				{
					dcheck( ImGui::SliderFloat( tag( "Glossiness" ), &v, 1.f, 100.f, "%.1f", 1.5f ) );
				}
				return dirty;
			}
		} probe;

		Accept( probe );
	}
	ImGui::End();
}