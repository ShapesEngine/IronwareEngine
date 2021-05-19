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
		meshPtrs.push_back( std::make_unique<Mesh>( gfx, materials[mesh.mMaterialIndex], mesh, scale ) );
	}

	pRoot = ParseNode( *pScene->mRootNode, scale );
	pRoot->SetAppliedTransform( DirectX::XMMatrixTranslationFromVector( DirectX::XMLoadFloat3( &startingPos ) ) );
}

void Model::Submit( size_t channelFilter ) const IFNOEXCEPT
{
	/*pModelWindow->ApplyParameters();*/
	pRoot->Submit( channelFilter, dx::XMMatrixIdentity() );
}

void Model::SetRootTransform( DirectX::FXMMATRIX tf ) noexcept
{
	pRoot->SetAppliedTransform( tf );
}

void Model::Accept( ModelProbe & probe )
{
	pRoot->Accept( probe );
}

void Model::LinkTechniques( RenderGraph & rg )
{
	for( auto& pMesh : meshPtrs )
	{
		pMesh->LinkTechniques( rg );
	}
}

Model::~Model() noexcept = default;

std::unique_ptr<Node> Model::ParseNode( const aiNode & node, float scale ) IFNOEXCEPT
{
	const auto parentTransform = scale_translation( dx::XMMatrixTranspose( dx::XMLoadFloat4x4( reinterpret_cast<const dx::XMFLOAT4X4*>( &node.mTransformation ) ) ), scale );

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
		pNode->AddChild( ParseNode( *node.mChildren[i], scale ) );
	}

	return pNode;
}