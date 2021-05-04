/*!
 * \file Material.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "Material.h"
#include "IronUtils.h"

Material::Material( Graphics& gfx, const aiMaterial& material, const std::filesystem::path& path ) IFNOEXCEPT :
	modelPath( path.wstring() )
{
	const auto rootPath = path.parent_path().wstring() + L"\\";
	{
		aiString tempName;
		material.Get( AI_MATKEY_NAME, tempName );
		name = to_wide( tempName.C_Str() );
	}
	// phong technique
	{
		RenderTechnique phong{ L"Phong" };
		RenderStep step( 0 );
		std::wstring shaderCode = L"Phong";
		aiString texFileName;

		// common (pre)
		vtxLayout.Append( VertexLayout::ElementType::Position3D );
		vtxLayout.Append( VertexLayout::ElementType::Normal );
		RawLayout pscLayout;
		bool hasTexture = false;
		bool hasGlossAlpha = false;

		// diffuse
		{
			bool hasAlpha = false;
			if( material.GetTexture( aiTextureType_DIFFUSE, 0, &texFileName ) == aiReturn_SUCCESS )
			{
				hasTexture = true;
				shaderCode += L"Dif";
				vtxLayout.Append( VertexLayout::ElementType::Texture2D );
				auto tex = Texture::Resolve( gfx, rootPath + to_wide( texFileName.C_Str() ) );
				if( tex->HasAlpha() )
				{
					hasAlpha = true;
					shaderCode += L"Msk";
				}
				step.AddBindable( std::move( tex ) );
			}
			else
			{
				pscLayout.Add<Float3>( "materialColor" );
			}
			step.AddBindable( RasterizerState::Resolve( gfx, hasAlpha ) );
		}
		// specular
		{
			if( material.GetTexture( aiTextureType_SPECULAR, 0, &texFileName ) == aiReturn_SUCCESS )
			{
				hasTexture = true;
				shaderCode += L"Spc";
				vtxLayout.Append( VertexLayout::ElementType::Texture2D );
				auto tex = Texture::Resolve( gfx, rootPath + to_wide( texFileName.C_Str() ), 1 );
				hasGlossAlpha = tex->HasAlpha();
				step.AddBindable( std::move( tex ) );
				pscLayout.Add<Bool>( "useGlossAlpha" );
			}
			pscLayout.Add<Float3>( "specularColor" );
			pscLayout.Add<Float>( "specularWeight" );
			pscLayout.Add<Float>( "specularGloss" );
		}
		// normal
		{
			if( material.GetTexture( aiTextureType_NORMALS, 0, &texFileName ) == aiReturn_SUCCESS )
			{
				hasTexture = true;
				shaderCode += L"Nrm";
				vtxLayout.Append( VertexLayout::ElementType::Texture2D );
				vtxLayout.Append( VertexLayout::ElementType::Tangent );
				vtxLayout.Append( VertexLayout::ElementType::Bitangent );
				step.AddBindable( Texture::Resolve( gfx, rootPath + to_wide( texFileName.C_Str() ), 2 ) );
				pscLayout.Add<Bool>( "useNormalMap" );
				pscLayout.Add<Float>( "normalMapWeight" );
			}
		}
		// common (post)
		{
			step.AddBindable( std::make_shared<TransformCBuffer>( gfx, 0u ) );
			step.AddBindable( BlendState::Resolve( gfx, false ) );
			auto pvs = VertexShader::Resolve( gfx, shaderCode + L"VS.cso" );
			auto pvsbc = pvs->GetBytecode();
			step.AddBindable( std::move( pvs ) );
			step.AddBindable( PixelShader::Resolve( gfx, shaderCode + L"PS.cso" ) );
			step.AddBindable( InputLayout::Resolve( gfx, vtxLayout, pvsbc ) );
			if( hasTexture )
			{
				step.AddBindable( Sampler::Resolve( gfx ) );
			}
			// PS material params (cbuf)
			Buffer buf{ std::move( pscLayout ) };
			if( auto r = buf["materialColor"]; r.Exists() )
			{
				aiColor3D color = { 0.45f,0.45f,0.85f };
				material.Get( AI_MATKEY_COLOR_DIFFUSE, color );
				r = reinterpret_cast<DirectX::XMFLOAT3&>( color );
			}
			buf["useGlossAlpha"].SetIfExists( hasGlossAlpha );
			if( auto r = buf["specularColor"]; r.Exists() )
			{
				aiColor3D color = { 0.18f,0.18f,0.18f };
				material.Get( AI_MATKEY_COLOR_SPECULAR, color );
				r = reinterpret_cast<DirectX::XMFLOAT3&>( color );
			}
			buf["specularWeight"].SetIfExists( 1.0f );
			if( auto r = buf["specularGloss"]; r.Exists() )
			{
				float gloss = 8.0f;
				material.Get( AI_MATKEY_SHININESS, gloss );
				r = gloss;
			}
			buf["useNormalMap"].SetIfExists( true );
			buf["normalMapWeight"].SetIfExists( 1.0f );
			step.AddBindable( std::make_unique<CachingPixelConstantBufferEx>( gfx, std::move( buf ), 1u ) );
		}
		phong.AddStep( std::move( step ) );
		techniques.push_back( std::move( phong ) );
	}
	// outline technique
	{
		RenderTechnique outline( L"Outline" );
		{
			RenderStep mask( 1 );

			auto pvs = VertexShader::Resolve( gfx, L"SolidVS.cso" );
			auto pvsbc = pvs->GetBytecode();
			mask.AddBindable( std::move( pvs ) );

			// TODO: better sub-layout generation tech for future consideration maybe
			mask.AddBindable( InputLayout::Resolve( gfx, vtxLayout, pvsbc ) );

			mask.AddBindable( std::make_shared<TransformCBuffer>( gfx ) );

			// TODO: might need to specify rasterizer when doubled-sided models start being used

			outline.AddStep( std::move( mask ) );
		}
		{
			RenderStep draw( 2 );

			// these can be pass-constant (tricky due to layout issues)
			auto pvs = VertexShader::Resolve( gfx, L"SolidVS.cso" );
			auto pvsbc = pvs->GetBytecode();
			draw.AddBindable( std::move( pvs ) );

			// this can be pass-constant
			draw.AddBindable( PixelShader::Resolve( gfx, L"SolidPS.cso" ) );

			RawLayout lay;
			lay.Add<Float4>( "materialColor" );
			auto buf = Buffer( std::move( lay ) );
			buf["materialColor"] = DirectX::XMFLOAT4{ 1.0f,0.4f,0.4f,1.0f };
			draw.AddBindable( std::make_shared<CachingPixelConstantBufferEx>( gfx, buf, 1u ) );

			// TODO: better sub-layout generation tech for future consideration maybe
			draw.AddBindable( InputLayout::Resolve( gfx, vtxLayout, pvsbc ) );

			// quick and dirty... nicer solution maybe takes a lamba... we'll see :)
			class TransformCbufScaling : public TransformCBuffer
			{
			public:
				TransformCbufScaling( Graphics& gfx, float scale = 1.04 )
					:
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
					const float scale = buf["scale"];
					const auto scaleMatrix = DirectX::XMMatrixScaling( scale, scale, scale );
					auto xf = GetTransform( gfx );
					xf.modelView = xf.modelView * scaleMatrix;
					xf.modelViewProj = xf.modelViewProj * scaleMatrix;
					UpdateBind( gfx, xf );
				}
				std::unique_ptr<CloningBindable> Clone() const noexcept override
				{
					return std::make_unique<TransformCbufScaling>( *this );
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
			draw.AddBindable( std::make_shared<TransformCbufScaling>( gfx ) );

			// TODO: might need to specify rasterizer when doubled-sided models start being used

			outline.AddStep( std::move( draw ) );
		}
		techniques.push_back( std::move( outline ) );
	}
}
VertexByteBuffer Material::ExtractVertices( const aiMesh& mesh ) const noexcept
{
	return { vtxLayout, mesh };
}
std::vector<RenderTechnique> Material::GetTechniques() const noexcept
{
	return techniques;
}