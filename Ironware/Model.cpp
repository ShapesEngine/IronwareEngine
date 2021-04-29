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
#include "DynamicConstantBuffer.h"
#include "ConstantBuffersEx.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <memory>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <filesystem>

namespace dx = DirectX;

Mesh::Mesh( Graphics& gfx, std::vector<std::shared_ptr<Bindable>> bindablePtrs )
{
	AddBind( PrimitiveTopology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	for( auto& pb : bindablePtrs )
	{
		AddBind( std::move( pb ) );
	}

	AddBind( std::make_shared<TransformCBuffer>( gfx, *this ) );
}

void Mesh::Draw( Graphics& gfx, dx::FXMMATRIX accumulatedTransform ) const IFNOEXCEPT
{
	dx::XMStoreFloat4x4( &transform, accumulatedTransform );
	Drawable::Draw( gfx );
}

/*!
 * \class Model
 *
 * \brief pImpl idiom class that controls window of a model
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 */
class ModelWindow
{
public:
	void ShowWindow( Graphics& gfx, const char* name, const Node& root ) IFNOEXCEPT
	{
		if( ImGui::Begin( name ) )
		{
			ImGui::Columns( 2, nullptr, true );
			//1st column
			root.ShowTree( pSelectedNode );

			ImGui::NextColumn();
			//2nd column
			if( pSelectedNode )
			{
				const auto id = pSelectedNode->GetID();
				auto i = transforms.find( id );
				if( i == transforms.end() )
				{
					TransformationParams tp;
					const auto& tf = pSelectedNode->GetAppliedTransform();

					const auto rot = extract_euler_angles( tf );
					tp.pitch = rot.x;
					tp.yaw = rot.y;
					tp.roll = rot.z;

					const auto translation = extract_translation( tf );
					tp.x = translation.x;
					tp.y = translation.y;
					tp.z = translation.z;

					std::tie( i, std::ignore ) = transforms.insert( { id, std::move( tp ) } );
				}

				auto& transform = i->second;
				ImGui::Text( "Position" );
				ImGui::SliderFloat( "X", &transform.x, -20.0f, 20.0f );
				ImGui::SliderFloat( "Y", &transform.y, -20.0f, 20.0f );
				ImGui::SliderFloat( "Z", &transform.z, -20.0f, 20.0f );

				ImGui::Text( "Orientation" );
				ImGui::SliderAngle( "Pitch", &transform.pitch, -180.0f, 180.0f );
				ImGui::SliderAngle( "Yaw", &transform.yaw, -180.0f, 180.0f );
				ImGui::SliderAngle( "Roll", &transform.roll, -180.0f, 180.0f );
				if( !pSelectedNode->ShowControlMaterial( gfx, matFull ) )
				{
					pSelectedNode->ShowControlMaterial( gfx, matNoTex );
				}
			}
			//==========
		}
		ImGui::End();
	}

	auto GetTransform() const noexcept
	{
		const auto& transform = transforms.at( pSelectedNode->index );
		return dx::XMMatrixRotationRollPitchYaw( transform.pitch, transform.yaw, transform.roll ) *
			dx::XMMatrixTranslation( transform.x, transform.y, transform.z );
	}

	Node* GetSelectedNode() const noexcept
	{
		return pSelectedNode;
	}

private:
	struct TransformationParams
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};
	std::unordered_map<uint32_t, TransformationParams> transforms;
	Node* pSelectedNode = nullptr;
	Node::PSMaterialConstantFull matFull;
	Node::PSMaterialConstantNoTex matNoTex;
};

Node::Node( std::vector<Mesh*> meshPtrs, const std::string& name, uint32_t index, const dx::XMMATRIX& transform_in ) IFNOEXCEPT :
	meshPtrs( std::move( meshPtrs ) ),
	name( name ),
	index( index )
{
	dx::XMStoreFloat4x4( &parentTransform, transform_in );
	dx::XMStoreFloat4x4( &appliedTransform, dx::XMMatrixIdentity() );
}

void Node::Draw( Graphics& gfx, dx::FXMMATRIX accumulatedTransform ) const IFNOEXCEPT
{
	const auto built =
		dx::XMLoadFloat4x4( &appliedTransform ) *
		dx::XMLoadFloat4x4( &parentTransform ) *
		accumulatedTransform;
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

void Node::ShowTree( Node*& pSelectedNode ) const IFNOEXCEPT
{
	const auto node_flags =
		( pSelectedNode != nullptr && index == pSelectedNode->index ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None ) |
		( childPtrs.empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow );

	const bool isExpanded = ImGui::TreeNodeEx( reinterpret_cast<void*>( (intptr_t)index ), node_flags, name.c_str() );
	if( ImGui::IsItemClicked() )
	{
		pSelectedNode = const_cast<Node*>( this );
	}

	if( isExpanded )
	{
		for( const auto& pc : childPtrs )
		{
			pc->ShowTree( pSelectedNode );
		}
		ImGui::TreePop();
	}
}

const char* Model::Exception::what() const noexcept
{
	std::wostringstream woss;
	woss << IronException::what() << std::endl
		<< "[Note] " << GetNote().c_str();
	whatBuffer = woss.str();
	return CON_CHREINT_CAST( whatBuffer.c_str() );
}

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
		throw Exception( __LINE__, WFILE, importer.GetErrorString() );
	}

	meshPtrs.reserve( pScene->mNumMeshes );
	for( size_t i = 0; i < pScene->mNumMeshes; i++ )
	{
		meshPtrs.push_back( ParseMesh( gfx, *pScene->mMeshes[i], pScene->mMaterials ) );
	}

	pRoot = ParseNode( *pScene->mRootNode );
	pRoot->SetAppliedTransform( DirectX::XMMatrixTranslationFromVector( DirectX::XMLoadFloat3( &startingPos ) ) );
}

void Model::Draw( Graphics& gfx ) const noexcept( !IS_DEBUG )
{
	if( auto node = pModelWindow->GetSelectedNode() )
	{
		node->SetAppliedTransform( pModelWindow->GetTransform() );
	}
	pRoot->Draw( gfx, dx::XMMatrixIdentity() );
}

void Model::ShowWindow( Graphics& gfx, const char* name ) const noexcept( !IS_DEBUG )
{
	pModelWindow->ShowWindow( gfx, name, *pRoot );
}

size_t Model::GetNodeSize() const noexcept
{
	return nodeNum;
}

Model::~Model() noexcept = default;

std::unique_ptr<Mesh> Model::ParseMesh( Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials ) IFNOEXCEPT
{
	using ElType = VertexLayout::ElementType;

	const std::wstring rootPath = std::move( std::filesystem::path( path ).parent_path().wstring() + L"\\" );
	const auto meshTag = path + L"$" + to_wide( mesh.mName.C_Str() );

	VertexByteBuffer vbuff{ 
		VertexLayout{} 
		.Append( ElType::Position3D )
		.Append( ElType::Normal )
	};

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

	std::vector<std::shared_ptr<Bindable>> bindablePtrs;

	bool hasDiffMap = false;
	bool hasSpecMap = false;
	bool hasNormalMap = false;
	bool hasAlphaGloss = false;
	bool hasAlphaDiffuse = false;

	aiColor4D diffuseColor = { 0.45f, 0.45f, 0.85f, 1.0f };
	aiColor4D specularColor = { 0.18f, 0.18f, 0.18f, 1.0f };
	ai_real shininess = 40.f;

	ID3DBlob* pVertShaderBytecode = nullptr;

	// index should be -1 if there is no material assigned to the mesh
	if( mesh.mMaterialIndex >= 0 )
	{
		const auto& material = *pMaterials[mesh.mMaterialIndex];
		aiString texPath;
		if( material.GetTexture( aiTextureType_DIFFUSE, 0u, &texPath ) == aiReturn_SUCCESS )
		{
			std::wstring wTexPath = rootPath + to_wide( std::string( texPath.C_Str() ) );
			auto pTex = Texture::Resolve( gfx, wTexPath );
			hasDiffMap = true;
			hasAlphaDiffuse = pTex->HasAlpha();
			bindablePtrs.push_back( std::move( pTex ) );
		}
		if( material.GetTexture( aiTextureType_NORMALS, 0u, &texPath ) == aiReturn_SUCCESS )
		{
			std::wstring wTexPath = rootPath + to_wide( std::string( texPath.C_Str() ) );
			auto pTex = Texture::Resolve( gfx, wTexPath, 2u );
			hasAlphaGloss = pTex->HasAlpha();
			hasNormalMap = true;
			bindablePtrs.push_back( std::move( pTex ) );
		}
		if( material.GetTexture( aiTextureType_SPECULAR, 0u, &texPath ) == aiReturn_SUCCESS )
		{
			std::wstring wTexPath = rootPath + to_wide( std::string( texPath.C_Str() ) );
			auto pTex = Texture::Resolve( gfx, wTexPath, 1u );
			hasAlphaGloss = pTex->HasAlpha();
			hasSpecMap = true;
			bindablePtrs.push_back( std::move( pTex ) );
		}

		if( hasDiffMap || hasNormalMap || hasSpecMap )
		{
			bindablePtrs.push_back( Sampler::Resolve( gfx ) );
		}
		else
		{
			if( !hasDiffMap && material.Get( AI_MATKEY_COLOR_DIFFUSE, diffuseColor ) != aiReturn_SUCCESS )
			{
				diffuseColor = { 0.45f, 0.45f, 0.85f, 1.0f };
			}
			if( !hasDiffMap && material.Get( AI_MATKEY_COLOR_SPECULAR, specularColor ) != aiReturn_SUCCESS )
			{
				specularColor = { 0.18f, 0.18f, 0.18f, 1.0f  };
			}
			if( !hasAlphaGloss && material.Get( AI_MATKEY_SHININESS, shininess ) != aiReturn_SUCCESS )
			{
				shininess = 40.f;
			}
		}
	}

	if( hasDiffMap && hasNormalMap )
	{
		vbuff.GetLayout()
			.Append( ElType::Texture2D )
			.Append( VertexLayout::ElementType::Tangent )
			.Append( VertexLayout::ElementType::Bitangent );

		for( uint32_t i = 0; i < mesh.mNumVertices; i++ )
		{
			vbuff.EmplaceBack(
				*reinterpret_cast<const dx::XMFLOAT3*>( &mesh.mVertices[i].operator*=( scale ) ), // mVertices is vertex positions
				*reinterpret_cast<dx::XMFLOAT3*>( &mesh.mNormals[i] ),
				*reinterpret_cast<dx::XMFLOAT2*>( &mesh.mTextureCoords[0][i] ),
				*reinterpret_cast<dx::XMFLOAT3*>( &mesh.mTangents[i] ),
				*reinterpret_cast<dx::XMFLOAT3*>( &mesh.mBitangents[i] )
			);
		}

		auto pVertShader = VertexShader::Resolve( gfx, L"PhongNormalMapVS.cso" );
		// save bytecode, as it will be needed in input layout
		pVertShaderBytecode = static_cast<VertexShader&>( *pVertShader ).GetBytecode();
		bindablePtrs.push_back( std::move( pVertShader ) );

		if( hasSpecMap )
		{
			bindablePtrs.push_back( PixelShader::Resolve( gfx, hasAlphaDiffuse ?
									L"PhongSpecNormalMapMaskPS.cso" : L"PhongSpecNormalMapPS.cso" ) );
			Node::PSMaterialConstantFull pMc;
			pMc.hasGlossMap = hasAlphaGloss ? TRUE : FALSE;
			pMc.specularPower = shininess;

			bindablePtrs.push_back( PixelConstantBuffer<Node::PSMaterialConstantFull>::Resolve( gfx, pMc, 1u ) );
		}
		else
		{
			struct PSMaterialConstantDiffNormal
			{
				float specularIntensity = 0.2f;
				float specularPower;
				alignas( 8 ) BOOL normalMapEnabled = TRUE;
			} pMc;

			pMc.specularIntensity = ( specularColor.r + specularColor.g + specularColor.b ) / 3.f;
			pMc.specularPower = shininess;

			bindablePtrs.push_back( PixelConstantBuffer<PSMaterialConstantDiffNormal>::Resolve( gfx, pMc, 1u ) );
			bindablePtrs.push_back( PixelShader::Resolve( gfx, L"PhongNormalMapPS.cso" ) );
		}
	}
	else if( hasDiffMap )
	{
		vbuff.GetLayout()
			.Append( ElType::Texture2D );

		for( uint32_t i = 0; i < mesh.mNumVertices; i++ )
		{
			vbuff.EmplaceBack(
				*reinterpret_cast<const dx::XMFLOAT3*>( &mesh.mVertices[i].operator*=( scale ) ), // mVertices is vertex positions
				*reinterpret_cast<dx::XMFLOAT3*>( &mesh.mNormals[i] ),
				*reinterpret_cast<dx::XMFLOAT2*>( &mesh.mTextureCoords[0][i] )
			);
		}

		auto pVertShader = VertexShader::Resolve( gfx, L"PhongDiffuseMapVS.cso" );
		// save bytecode, as it will be needed in input layout
		pVertShaderBytecode = static_cast<VertexShader&>( *pVertShader ).GetBytecode();

		bindablePtrs.push_back( std::move( pVertShader ) );

		if( hasSpecMap )
		{
			struct PSMaterialConstantDiffSpec
			{
				float specularPower;
				float specularMapWeight;
				alignas( 8 ) BOOL hasGloss;
			} pMc;

			pMc.specularPower = shininess;
			pMc.specularMapWeight = 1.f;
			pMc.hasGloss = hasAlphaGloss ? TRUE : FALSE;

			bindablePtrs.push_back( PixelConstantBuffer<PSMaterialConstantDiffSpec>::Resolve( gfx, pMc, 1u ) );
			bindablePtrs.push_back( PixelShader::Resolve( gfx, L"PhongSpecMapPS.cso" ) );
		}
		else
		{
			struct PSMaterialConstantDiff
			{
				float specularIntensity = 0.2f;
				float specularPower;
				alignas( 8 ) float padding;
			} pMc;

			pMc.specularIntensity = ( specularColor.r + specularColor.g + specularColor.b ) / 3.f;
			pMc.specularPower = shininess;

			bindablePtrs.push_back( PixelConstantBuffer<PSMaterialConstantDiff>::Resolve( gfx, pMc, 1u ) );
			bindablePtrs.push_back( PixelShader::Resolve( gfx, L"PhongDiffuseMapPS.cso" ) );
		}
	}
	else if( !hasDiffMap && !hasSpecMap && !hasNormalMap )
	{
		for( uint32_t i = 0; i < mesh.mNumVertices; i++ )
		{
			vbuff.EmplaceBack(
				*reinterpret_cast<const dx::XMFLOAT3*>( &mesh.mVertices[i].operator*=( scale ) ), // mVertices is vertex positions
				*reinterpret_cast<dx::XMFLOAT3*>( &mesh.mNormals[i] )
			);
		}

		Node::PSMaterialConstantNoTex pMc;

		pMc.materialColor = reinterpret_cast<dx::XMFLOAT4&>( diffuseColor );
		pMc.specularColor = reinterpret_cast<dx::XMFLOAT4&>( specularColor );
		pMc.specularPower = shininess;

		bindablePtrs.push_back( PixelConstantBuffer<Node::PSMaterialConstantNoTex>::Resolve( gfx, pMc, 1u ) );

		auto pVertShader = VertexShader::Resolve( gfx, L"PhongVS.cso" );
		// save bytecode, as it will be needed in input layout
		ID3DBlob* pVertShaderBytecode = static_cast<VertexShader&>( *pVertShader ).GetBytecode();
		bindablePtrs.push_back( std::move( pVertShader ) );
		bindablePtrs.push_back( PixelShader::Resolve( gfx, L"PhongPS.cso" ) );
	}

	bindablePtrs.push_back( VertexBuffer::Resolve( gfx, meshTag, vbuff ) );
	bindablePtrs.push_back( IndexBuffer::Resolve( gfx, meshTag, indices ) );
	if( pVertShaderBytecode )
	{
		bindablePtrs.push_back( InputLayout::Resolve( gfx, vbuff.GetLayout(), pVertShaderBytecode ) );
	}

	bindablePtrs.push_back( BlendState::Resolve( gfx, false ) );
	// it's assumed that all models that have alpha value in diffuse tex to be two sided meshes
	bindablePtrs.push_back( RasterizerState::Resolve( gfx, hasAlphaDiffuse ) );

	return std::make_unique<Mesh>( gfx, std::move( bindablePtrs ) );
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