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
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <memory>
#include <vector>

Model::Model( Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	DirectX::XMFLOAT3 material, float scale ) :
	ObjectBase( gfx, rng, adist, ddist, odist, rdist )
{
	namespace dx = DirectX;

	if( !IsStaticInitialized() )
	{
		using ElType = VertexLayout::ElementType;
		VertexByteBuffer vbuff(
			VertexLayout{}
			.Append( ElType::Position3D )
			.Append( ElType::Normal )
		);

		Assimp::Importer importer;
		auto pModel = importer.ReadFile( "Models\\suzanne.obj", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices );

		// 0, as there is only 1 mesh in the file
		auto pMesh = pModel->mMeshes[0];
		const auto meshVerticesNum = pMesh->mNumVertices;

		for( uint32_t i = 0; i < meshVerticesNum; i++ )
		{
			vbuff.EmplaceBack( dx::XMFLOAT3{ pMesh->mVertices[i].x * scale, pMesh->mVertices[i].y * scale, pMesh->mVertices[i].z * scale },
				*reinterpret_cast<dx::XMFLOAT3*>( &pMesh->mNormals[i] ) );
		}

		AddStaticBind( std::make_unique<VertexBuffer>( gfx, vbuff ) );

		auto pVertShader = std::make_unique<VertexShader>( gfx, L"PhongVS.cso" );
		// save bytecode, as it will be needed in input layout
		auto pVertShaderBytecode = pVertShader->GetBytecode();
		AddStaticBind( std::move( pVertShader ) );

		AddStaticBind( std::make_unique<PixelShader>( gfx, L"PhongPS.cso" ) );

		std::vector<uint16_t> indices;
		indices.reserve( (size_t)pMesh->mNumFaces * 3 );

		for( uint16_t i = 0; i < pMesh->mNumFaces; i++ )
		{
			const auto& face = pMesh->mFaces[i];
			assert( face.mNumIndices == 3 );
			indices.push_back( face.mIndices[0] );
			indices.push_back( face.mIndices[1] );
			indices.push_back( face.mIndices[2] );
		}

		AddStaticIndexBufferBind( std::make_unique<IndexBuffer>( gfx, indices ) );

		AddStaticBind( std::make_unique<InputLayout>( gfx, vbuff.GetLayout().GetD3DLayout(), pVertShaderBytecode ) );

		AddStaticBind( std::make_unique<PrimitiveTopology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

		struct PSMaterialConstant
		{
			DirectX::XMFLOAT3 color;
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[3];
		} pMc;
		pMc.color = material;
		AddStaticBind( std::make_unique<PixelConstantBuffer<PSMaterialConstant>>( gfx, pMc, 1u ) );
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformCBuffer>( gfx, *this ) );
}