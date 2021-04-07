/*!
 * \file Model.cpp
 *
 * \author Yernar Aldabergenov
 * \date March 2021
 *
 *
 */
#include "Model.h"
#include "BindableCommon.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <memory>
#include <vector>

Mesh::Mesh( Graphics& gfx, std::vector<std::unique_ptr<Bindable>> bindablePtrs )
{
	if( !IsStaticInitialized() )
	{
		AddStaticBind( std::make_unique<PrimitiveTopology>( gfx ) );
	}

	for( auto& pb : bindablePtrs )
	{
		if( const auto pi = dynamic_cast<IndexBuffer*>( pb.get() ) )
		{
			AddIndexBufferBind( std::unique_ptr<IndexBuffer>{ pi } );
			pb.release();
		}
		else
		{
			AddBind( std::move( pb ) );
		}
	}

	AddBind( std::make_unique<TransformCBuffer>( gfx, *this ) );
}

void Mesh::Draw( Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform ) const IFNOEXCEPT
{
	DirectX::XMStoreFloat4x4( &transform, accumulatedTransform );
	Drawable::Draw( gfx );
}

Node::Node( std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in ) :
	meshPtrs( std::move( meshPtrs ) )
{
	DirectX::XMStoreFloat4x4( &transform, transform_in );
}

void Node::Draw( Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform ) const IFNOEXCEPT
{
	const auto built = DirectX::XMLoadFloat4x4( &transform ) * accumulatedTransform;
	for( const auto pm : meshPtrs )
	{
		pm->Draw( gfx, built );
	}

	for( const auto& pc : childPtrs )
	{
		pc->Draw( gfx, built );
	}
}

void Node::AddChild( std::unique_ptr<Node> pChild ) IFNOEXCEPT
{
	assert( pChild );
	childPtrs.push_back( std::move( pChild ) );
}

Model::Model( Graphics & gfx, std::string filename )
{
	Assimp::Importer importer;
	auto pModel = importer.ReadFile( filename, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices );

	meshPtrs.reserve( pModel->mNumMeshes );
	for( size_t i = 0; i < pModel->mNumMeshes; i++ )
	{
		meshPtrs.push_back( ParseMesh( gfx, *pModel->mMeshes[i] ) );
	}

	pRoot = ParseNode( *pModel->mRootNode );
}

std::unique_ptr<Mesh> Model::ParseMesh( Graphics& gfx, const aiMesh& mesh ) IFNOEXCEPT
{
	using ElType = VertexLayout::ElementType;
	VertexByteBuffer vbuff(
		VertexLayout{}
		.Append( ElType::Position3D )
		.Append( ElType::Normal )
	);

	for( uint32_t i = 0; i < mesh.mNumVertices; i++ )
	{
		vbuff.EmplaceBack(
			*reinterpret_cast<DirectX::XMFLOAT3*>( &mesh.mVertices[i] ), // mVertices is vertex positions
			*reinterpret_cast<DirectX::XMFLOAT3*>( &mesh.mNormals[i] )
		);
	}

	std::vector<uint16_t> indices;
	indices.reserve( (size_t)mesh.mNumFaces * 3u );
	for( uint32_t i = 0u; i < mesh.mNumFaces; i++ )
	{
		const auto& face = mesh.mFaces[i];
		assert( face.mNumIndices == 3 );
		indices.push_back( face.mIndices[0] );
		indices.push_back( face.mIndices[1] );
		indices.push_back( face.mIndices[2] );
	}

	std::vector<std::unique_ptr<Bindable>> bindablePtrs;

	bindablePtrs.push_back( std::make_unique<VertexBuffer>( gfx, vbuff ) );

	auto pVertShader = std::make_unique<VertexShader>( gfx, L"PhongVS.cso" );
	// save bytecode, as it will be needed in input layout
	auto pVertShaderBytecode = pVertShader->GetBytecode();
	bindablePtrs.push_back( std::move( pVertShader ) );

	bindablePtrs.push_back( std::make_unique<PixelShader>( gfx, L"PhongPS.cso" ) );

	bindablePtrs.push_back( std::make_unique<IndexBuffer>( gfx, indices ) );

	bindablePtrs.push_back( std::make_unique<InputLayout>( gfx, vbuff.GetLayout().GetD3DLayout(), pVertShaderBytecode ) );

	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 color = { 0.6f, 0.6f, 0.8f };
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	} pMc;

	bindablePtrs.push_back( std::make_unique<PixelConstantBuffer<PSMaterialConstant>>( gfx, pMc, 1u ) );

	return std::make_unique<Mesh>( gfx, std::move( bindablePtrs ) );
}

std::unique_ptr<Node> Model::ParseNode( const aiNode& node ) IFNOEXCEPT
{
	const auto transform = DirectX::XMMatrixTranspose( DirectX::XMLoadFloat4x4( reinterpret_cast<const DirectX::XMFLOAT4X4*>( &node.mTransformation ) ) );

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve( (size_t)node.mNumMeshes );
	for( uint32_t i = 0; i < node.mNumMeshes; i++ )
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back( meshPtrs.at( meshIdx ).get() );
	}

	auto pNode = std::make_unique<Node>( std::move( curMeshPtrs ), transform );

	for( uint32_t i = 0; i < node.mNumChildren; i++ )
	{
		pNode->AddChild( ParseNode( *node.mChildren[i] ) );
	}

	return pNode;
}