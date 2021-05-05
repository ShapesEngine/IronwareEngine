/*!
 * \file Model.cpp
 *
 * \author Yernar Aldabergenov
 * \date March 2021
 *
 *
 */
#include "Model.h"
#define IR_INCLUDE_TEXTURE
#include "BindableCommon.h"
#include "IronUtils.h"
#include "IronMath.h"
#include "ConstantBuffersEx.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <memory>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <filesystem>
#include "Node.h"
#include "ModelWindow.h"
#include "ModelException.h"
#include "Mesh.h"
#include "Material.h"

namespace dx = DirectX;

Model::Model( Graphics& gfx, std::wstring path, float scale, DirectX::XMFLOAT3 startingPos ) :
	scale( scale ),
	path( path )
{
	Assimp::Importer importer;
	auto pScene = importer.ReadFile(
		to_narrow( path ),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	if( !pScene )
	{
		throw ModelException( __LINE__, WFILE, importer.GetErrorString() );
	}

	// parse materials
	std::vector<Material> materials;
	materials.reserve( pScene->mNumMaterials );
	for( size_t i = 0; i < pScene->mNumMaterials; i++ )
	{
		materials.emplace_back( gfx, *pScene->mMaterials[i], path );
	}

	meshPtrs.reserve( pScene->mNumMeshes );
	for( size_t i = 0; i < pScene->mNumMeshes; i++ )
	{
		const auto& mesh = *pScene->mMeshes[i];
		meshPtrs.push_back( std::make_unique<Mesh>( gfx, materials[mesh.mMaterialIndex], mesh ));
	}

	pRoot = ParseNode( *pScene->mRootNode );
	pRoot->SetAppliedTransform( DirectX::XMMatrixTranslationFromVector( DirectX::XMLoadFloat3( &startingPos ) ) );
}

void Model::Submit( FrameExecutor& frame ) const IFNOEXCEPT
{
	/*pModelWindow->ApplyParameters();*/
	pRoot->Submit( frame, dx::XMMatrixIdentity() );
}

//void Model::ShowWindow( Graphics& gfx, const char* name ) const IFNOEXCEPT
//{
//	pModelWindow->ShowWindow( gfx, name, *pRoot );
//}

Model::~Model() noexcept = default;

std::unique_ptr<Mesh> Model::ParseMesh( Graphics & gfx, const aiMesh & mesh, const aiMaterial* const* pMaterials ) IFNOEXCEPT
{
	return{};
	//using ElType = VertexLayout::ElementType;

	//const std::wstring rootPath = std::move( std::filesystem::path( path ).parent_path().wstring() + L"\\" );
	//const auto meshTag = path + L"$" + to_wide( mesh.mName.C_Str() );

	//VertexByteBuffer vbuff{
	//	VertexLayout{}
	//	.Append( ElType::Position3D )
	//	.Append( ElType::Normal )
	//};

	//std::vector<uint16_t> indices;
	//indices.reserve( (size_t)mesh.mNumFaces * 3u );
	//for( uint32_t i = 0u; i < mesh.mNumFaces; i++ )
	//{
	//	const auto& face = mesh.mFaces[i];
	//	assert( face.mNumIndices == 3 );
	//	indices.push_back( face.mIndices[0] );
	//	indices.push_back( face.mIndices[1] );
	//	indices.push_back( face.mIndices[2] );
	//}

	//std::vector<std::shared_ptr<Bindable>> bindablePtrs;

	//bool hasDiffMap = false;
	//bool hasSpecMap = false;
	//bool hasNormalMap = false;
	//bool hasAlphaGloss = false;
	//bool hasAlphaDiffuse = false;

	//aiColor4D diffuseColor = { 0.45f, 0.45f, 0.85f, 1.f };
	//aiColor4D specularColor = { 0.18f, 0.18f, 0.18f, 1.f };
	//ai_real shininess = 40.f;

	//ID3DBlob* pVertShaderBytecode = nullptr;

	//// index should be -1 if there is no material assigned to the mesh
	//if( mesh.mMaterialIndex >= 0 )
	//{
	//	const auto& material = *pMaterials[mesh.mMaterialIndex];
	//	aiString texPath;
	//	if( material.GetTexture( aiTextureType_DIFFUSE, 0u, &texPath ) == aiReturn_SUCCESS )
	//	{
	//		std::wstring wTexPath = rootPath + to_wide( std::string( texPath.C_Str() ) );
	//		auto pTex = Texture::Resolve( gfx, wTexPath );
	//		hasDiffMap = true;
	//		hasAlphaDiffuse = pTex->HasAlpha();
	//		bindablePtrs.push_back( std::move( pTex ) );
	//	}
	//	if( material.GetTexture( aiTextureType_NORMALS, 0u, &texPath ) == aiReturn_SUCCESS )
	//	{
	//		std::wstring wTexPath = rootPath + to_wide( std::string( texPath.C_Str() ) );
	//		auto pTex = Texture::Resolve( gfx, wTexPath, 2u );
	//		hasAlphaGloss = pTex->HasAlpha();
	//		hasNormalMap = true;
	//		bindablePtrs.push_back( std::move( pTex ) );
	//	}
	//	if( material.GetTexture( aiTextureType_SPECULAR, 0u, &texPath ) == aiReturn_SUCCESS )
	//	{
	//		std::wstring wTexPath = rootPath + to_wide( std::string( texPath.C_Str() ) );
	//		auto pTex = Texture::Resolve( gfx, wTexPath, 1u );
	//		hasAlphaGloss = pTex->HasAlpha();
	//		hasSpecMap = true;
	//		bindablePtrs.push_back( std::move( pTex ) );
	//	}

	//	if( hasDiffMap || hasNormalMap || hasSpecMap )
	//	{
	//		bindablePtrs.push_back( Sampler::Resolve( gfx ) );
	//	}
	//	else
	//	{
	//		if( !hasDiffMap && material.Get( AI_MATKEY_COLOR_DIFFUSE, diffuseColor ) != aiReturn_SUCCESS )
	//		{
	//			diffuseColor = { 0.45f, 0.45f, 0.85f, 1.f };
	//		}
	//		if( !hasDiffMap && material.Get( AI_MATKEY_COLOR_SPECULAR, specularColor ) != aiReturn_SUCCESS )
	//		{
	//			specularColor = { 0.18f, 0.18f, 0.18f, 1.f };
	//		}
	//		if( !hasAlphaGloss && material.Get( AI_MATKEY_SHININESS, shininess ) != aiReturn_SUCCESS )
	//		{
	//			shininess = 40.f;
	//		}
	//	}
	//}

	//if( hasDiffMap && hasNormalMap )
	//{
	//	vbuff.GetLayout()
	//		.Append( ElType::Texture2D )
	//		.Append( VertexLayout::ElementType::Tangent )
	//		.Append( VertexLayout::ElementType::Bitangent );

	//	for( uint32_t i = 0; i < mesh.mNumVertices; i++ )
	//	{
	//		vbuff.EmplaceBack(
	//			*reinterpret_cast<const dx::XMFLOAT3*>( &mesh.mVertices[i].operator*=( scale ) ), // mVertices is vertex positions
	//			*reinterpret_cast<dx::XMFLOAT3*>( &mesh.mNormals[i] ),
	//			*reinterpret_cast<dx::XMFLOAT2*>( &mesh.mTextureCoords[0][i] ),
	//			*reinterpret_cast<dx::XMFLOAT3*>( &mesh.mTangents[i] ),
	//			*reinterpret_cast<dx::XMFLOAT3*>( &mesh.mBitangents[i] )
	//		);
	//	}

	//	auto pVertShader = VertexShader::Resolve( gfx, L"PhongNormalMapVS.cso" );
	//	// save bytecode, as it will be needed in input layout
	//	pVertShaderBytecode = static_cast<VertexShader&>( *pVertShader ).GetBytecode();
	//	bindablePtrs.push_back( std::move( pVertShader ) );

	//	if( hasSpecMap )
	//	{
	//		RawLayout lay;
	//		lay.Add<Type::Bool>( "normalMapEnabled" );
	//		lay.Add<Type::Bool>( "specularMapEnabled" );
	//		lay.Add<Type::Bool>( "hasGlossMap" );
	//		lay.Add<Type::Float>( "specularPower" );
	//		lay.Add<Type::Float3>( "specularColor" );
	//		lay.Add<Type::Float>( "specularMapWeight" );

	//		auto buf = Buffer( std::move( lay ) );
	//		buf["normalMapEnabled"] = true;
	//		buf["specularMapEnabled"] = true;
	//		buf["hasGlossMap"] = hasAlphaGloss;
	//		buf["specularPower"] = shininess;
	//		buf["specularColor"] = dx::XMFLOAT3{ 0.75f,0.75f,0.75f };
	//		buf["specularMapWeight"] = 0.671f;

	//		bindablePtrs.push_back( std::make_shared<CachingPixelConstantBufferEx>( gfx, buf, 1u ) );
	//		bindablePtrs.push_back( PixelShader::Resolve( gfx, hasAlphaDiffuse ?
	//			L"PhongSpecNormalMapMaskPS.cso" : L"PhongSpecNormalMapPS.cso" ) );
	//	}
	//	else
	//	{
	//		RawLayout layout;
	//		layout.Add<Type::Float>( "specularIntensity" );
	//		layout.Add<Type::Float>( "specularPower" );
	//		layout.Add<Type::Bool>( "normalMapEnabled" );

	//		auto cbuf = Buffer( std::move( layout ) );
	//		cbuf["specularIntensity"] = ( specularColor.r + specularColor.g + specularColor.b ) / 3.0f;
	//		cbuf["specularPower"] = shininess;
	//		cbuf["normalMapEnabled"] = true;

	//		bindablePtrs.push_back( std::make_shared<CachingPixelConstantBufferEx>( gfx, cbuf, 1u ) );
	//		bindablePtrs.push_back( PixelShader::Resolve( gfx, L"PhongNormalMapPS.cso" ) );
	//	}
	//}
	//else if( hasDiffMap )
	//{
	//	vbuff.GetLayout()
	//		.Append( ElType::Texture2D );

	//	for( uint32_t i = 0; i < mesh.mNumVertices; i++ )
	//	{
	//		vbuff.EmplaceBack(
	//			*reinterpret_cast<const dx::XMFLOAT3*>( &mesh.mVertices[i].operator*=( scale ) ), // mVertices is vertex positions
	//			*reinterpret_cast<dx::XMFLOAT3*>( &mesh.mNormals[i] ),
	//			*reinterpret_cast<dx::XMFLOAT2*>( &mesh.mTextureCoords[0][i] )
	//		);
	//	}

	//	auto pVertShader = VertexShader::Resolve( gfx, L"PhongDiffuseMapVS.cso" );
	//	// save bytecode, as it will be needed in input layout
	//	pVertShaderBytecode = static_cast<VertexShader&>( *pVertShader ).GetBytecode();

	//	bindablePtrs.push_back( std::move( pVertShader ) );

	//	if( hasSpecMap )
	//	{
	//		RawLayout lay;
	//		lay.Add<Type::Float>( "specularPower" );
	//		lay.Add<Type::Bool>( "hasGloss" );
	//		lay.Add<Type::Float>( "specularMapWeight" );

	//		auto buf = Buffer( std::move( lay ) );
	//		buf["specularPower"] = shininess;
	//		buf["hasGloss"] = hasAlphaGloss;
	//		buf["specularMapWeight"] = 1.0f;
	//		bindablePtrs.push_back( std::make_unique<CachingPixelConstantBufferEx>( gfx, buf, 1u ) );
	//		bindablePtrs.push_back( PixelShader::Resolve( gfx, L"PhongSpecMapPS.cso" ) );
	//	}
	//	else
	//	{
	//		RawLayout lay;
	//		lay.Add<Type::Float>( "specularIntensity" );
	//		lay.Add<Type::Float>( "specularPower" );

	//		auto buf = Buffer( std::move( lay ) );
	//		buf["specularIntensity"] = ( specularColor.r + specularColor.g + specularColor.b ) / 3.0f;
	//		buf["specularPower"] = shininess;
	//		buf["specularMapWeight"] = 1.0f;

	//		bindablePtrs.push_back( std::make_unique<CachingPixelConstantBufferEx>( gfx, buf, 1u ) );
	//		bindablePtrs.push_back( PixelShader::Resolve( gfx, L"PhongDiffuseMapPS.cso" ) );
	//	}
	//}
	//else if( !hasDiffMap && !hasSpecMap && !hasNormalMap )
	//{
	//	for( uint32_t i = 0; i < mesh.mNumVertices; i++ )
	//	{
	//		vbuff.EmplaceBack(
	//			*reinterpret_cast<const dx::XMFLOAT3*>( &mesh.mVertices[i].operator*=( scale ) ), // mVertices is vertex positions
	//			*reinterpret_cast<dx::XMFLOAT3*>( &mesh.mNormals[i] )
	//		);
	//	}

	//	RawLayout lay;
	//	lay.Add<Type::Float4>( "materialColor" );
	//	lay.Add<Type::Float4>( "specularColor" );
	//	lay.Add<Type::Float>( "specularPower" );

	//	auto buf = Buffer( std::move( lay ) );
	//	buf["specularPower"] = shininess;
	//	buf["specularColor"] = reinterpret_cast<dx::XMFLOAT4&>( specularColor );
	//	buf["materialColor"] = reinterpret_cast<dx::XMFLOAT4&>( diffuseColor );

	//	bindablePtrs.push_back( std::make_unique<CachingPixelConstantBufferEx>( gfx, buf, 1u ) );

	//	auto pVertShader = VertexShader::Resolve( gfx, L"PhongVS.cso" );
	//	// save bytecode, as it will be needed in input layout
	//	ID3DBlob* pVertShaderBytecode = static_cast<VertexShader&>( *pVertShader ).GetBytecode();
	//	bindablePtrs.push_back( std::move( pVertShader ) );
	//	bindablePtrs.push_back( PixelShader::Resolve( gfx, L"PhongPS.cso" ) );
	//}

	//bindablePtrs.push_back( VertexBuffer::Resolve( gfx, meshTag, vbuff ) );
	//bindablePtrs.push_back( IndexBuffer::Resolve( gfx, meshTag, indices ) );
	//if( pVertShaderBytecode )
	//{
	//	bindablePtrs.push_back( InputLayout::Resolve( gfx, vbuff.GetLayout(), pVertShaderBytecode ) );
	//}

	//bindablePtrs.push_back( BlendState::Resolve( gfx, false ) );
	//// it's assumed that all models that have alpha value in diffuse tex to be two sided meshes
	//bindablePtrs.push_back( RasterizerState::Resolve( gfx, hasAlphaDiffuse ) );

	//bindablePtrs.push_back( std::make_shared<DepthStencilState>( gfx, DepthStencilState::StencilMode::Off ) );

	//return std::make_unique<Mesh>( gfx, std::move( bindablePtrs ) );
}

std::unique_ptr<Node> Model::ParseNode( const aiNode & node ) IFNOEXCEPT
{
	const auto parentTransform = dx::XMMatrixTranspose( dx::XMLoadFloat4x4( reinterpret_cast<const dx::XMFLOAT4X4*>( &node.mTransformation ) ) );

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve( (size_t)node.mNumMeshes );
	for( uint32_t i = 0; i < node.mNumMeshes; i++ )
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back( meshPtrs.at( meshIdx ).get() );
	}

	auto pNode = std::make_unique<Node>( std::move( curMeshPtrs ), node.mName.C_Str(), (uint32_t)nodeNum++, parentTransform );

	for( uint32_t i = 0; i < node.mNumChildren; i++ )
	{
		pNode->AddChild( ParseNode( *node.mChildren[i] ) );
	}

	return pNode;
}