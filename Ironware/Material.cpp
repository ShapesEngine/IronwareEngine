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
#include "TransformCBufferScaling.h"
#include "DepthStencilState.h"
#include "IronChannels.h"

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
		RenderTechnique phong{ L"Phong", IR_CH::main };
		RenderStep step( "lambertian" );
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
				pscLayout.Add<Bool>( "useSpecMap" );
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
			auto pvs = VertexShader::Resolve( gfx, shaderCode + L"_VS.cso" );
			step.AddBindable( InputLayout::Resolve( gfx, vtxLayout, *pvs ) );
			step.AddBindable( std::move( pvs ) );
			step.AddBindable( PixelShader::Resolve( gfx, shaderCode + L"_PS.cso" ) );
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
			buf["useSpecMap"].SetIfExists( true );
			if( auto r = buf["specularColor"]; r.Exists() )
			{
				aiColor3D color = { 0.18f,0.18f,0.18f };
				material.Get( AI_MATKEY_COLOR_SPECULAR, color );
				r = reinterpret_cast<DirectX::XMFLOAT3&>( color );
			}
			buf["specularWeight"].SetIfExists( 1.f );
			if( auto r = buf["specularGloss"]; r.Exists() )
			{
				float gloss = 8.f;
				material.Get( AI_MATKEY_SHININESS, gloss );
				r = gloss;
			}
			buf["useNormalMap"].SetIfExists( true );
			buf["normalMapWeight"].SetIfExists( 1.f );
			step.AddBindable( std::make_unique<CachingPixelConstantBufferEx>( gfx, std::move( buf ), 1u ) );
		}
		phong.AddStep( std::move( step ) );
		techniques.push_back( std::move( phong ) );
	}
	// outline technique
	{
		RenderTechnique outline( L"Outline", IR_CH::main, false );
		{
			RenderStep mask( "outlineMask" );

			mask.AddBindable( InputLayout::Resolve( gfx, vtxLayout, *VertexShader::Resolve( gfx, L"Solid_VS.cso" ) ) );

			mask.AddBindable( std::make_shared<TransformCBuffer>( gfx ) );

			outline.AddStep( std::move( mask ) );
		}
		{
			RenderStep draw( "outlineDraw" );

			// this can be pass-constant
			draw.AddBindable( PixelShader::Resolve( gfx, L"Solid_PS.cso" ) );

			{
				RawLayout lay;
				lay.Add<Float3>( "materialColor" );
				auto buf = Buffer( std::move( lay ) );
				buf["materialColor"] = DirectX::XMFLOAT3{ 1.f,1.f,0.5f };
				draw.AddBindable( std::make_shared<CachingPixelConstantBufferEx>( gfx, buf, 1u ) );
			}

			draw.AddBindable( InputLayout::Resolve( gfx, vtxLayout, *VertexShader::Resolve( gfx, L"Solid_VS.cso" ) ) );

			draw.AddBindable( std::make_shared<TransformCBuffer>( gfx ) );

			outline.AddStep( std::move( draw ) );
		}
		techniques.push_back( std::move( outline ) );
	}
	// shadow map technique
	/*{
		RenderTechnique map{ L"ShadowMap", IR_CH::shadow, true };
		{
			RenderStep draw( "shadowMap" );

			draw.AddBindable( InputLayout::Resolve( gfx, vtxLayout, *VertexShader::Resolve( gfx, L"Solid_VS.cso" ) ) );

			draw.AddBindable( std::make_shared<TransformCBuffer>( gfx ) );

			map.AddStep( std::move( draw ) );
		}
		techniques.push_back( std::move( map ) );
	}*/
}

VertexByteBuffer Material::ExtractVertices( const aiMesh& mesh ) const noexcept
{
	return { vtxLayout, mesh };
}

std::vector<uint16_t> Material::ExtractIndices( const aiMesh & mesh ) const noexcept
{
	std::vector<uint16_t> indices;
	indices.reserve( (size_t)mesh.mNumFaces * 3 );
	for( uint32_t i = 0; i < mesh.mNumFaces; i++ )
	{
		const auto& face = mesh.mFaces[i];
		assert( face.mNumIndices == 3 );
		indices.push_back( face.mIndices[0] );
		indices.push_back( face.mIndices[1] );
		indices.push_back( face.mIndices[2] );
	}
	return indices;
}

std::shared_ptr<VertexBuffer> Material::MakeVertexBindable( Graphics & gfx, const aiMesh & mesh, float scale ) const noexcept( !IS_DEBUG )
{
	auto vtc = ExtractVertices( mesh );
	if( scale != 1.f )
	{
		for( auto i = 0u; i < vtc.Size(); i++ )
		{
			DirectX::XMFLOAT3& pos = vtc[i].Attribute<VertexLayout::ElementType::Position3D>();
			pos.x *= scale;
			pos.y *= scale;
			pos.z *= scale;
		}
	}
	return VertexBuffer::Resolve( gfx, MakeMeshTag( mesh ), std::move( vtc ) );
}

std::shared_ptr<IndexBuffer> Material::MakeIndexBindable( Graphics & gfx, const aiMesh & mesh ) const noexcept( !IS_DEBUG )
{
	return IndexBuffer::Resolve( gfx, MakeMeshTag( mesh ), ExtractIndices( mesh ) );
}

std::vector<RenderTechnique> Material::GetTechniques() const noexcept
{
	return techniques;
}