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
#include <imgui/imgui.h>

#include <memory>
#include <vector>
#include <unordered_map>

namespace dx = DirectX;

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
	void ShowWindow( const char* name, const Node& root ) IFNOEXCEPT
	{
		if( ImGui::Begin( name ) )
		{
			ImGui::Columns( 2, nullptr, true );
			//1st column
			root.ShowTree( selectedIndex, pSelectedNode );

			ImGui::NextColumn();
			//2nd column
			if( const auto i = selectedIndex )
			{
				auto& transform = transforms[*i];
				ImGui::Text( "Orientation" );
				ImGui::SliderAngle( "Roll", &transform.roll, -180.0f, 180.0f );
				ImGui::SliderAngle( "Pitch", &transform.pitch, -180.0f, 180.0f );
				ImGui::SliderAngle( "Yaw", &transform.yaw, -180.0f, 180.0f );

				ImGui::Text( "Position" );
				ImGui::SliderFloat( "X", &transform.x, -20.0f, 20.0f );
				ImGui::SliderFloat( "Y", &transform.y, -20.0f, 20.0f );
				ImGui::SliderFloat( "Z", &transform.z, -20.0f, 20.0f );
			}
			//==========
		}
		ImGui::End();
	}

	auto GetTransform() const noexcept
	{
		const auto& transform = transforms.at( *selectedIndex );
		return dx::XMMatrixRotationRollPitchYaw( transform.pitch, transform.yaw, transform.roll ) *
			dx::XMMatrixTranslation( transform.x, transform.y, transform.z );
	}

	Node* GetSelectedNode() const noexcept
	{
		return pSelectedNode;
	}

public:
	static constexpr uint32_t UNUSED_INDEX = 9999;

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
	std::optional<uint32_t> selectedIndex;
};

Node::Node( std::vector<Mesh*> meshPtrs, const std::string& name, uint32_t index, const dx::XMMATRIX& transform_in ) :
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

void Node::SetAppliedTransform( dx::FXMMATRIX transform ) noexcept
{
	dx::XMStoreFloat4x4( &appliedTransform, transform );
}

void Node::ShowTree( std::optional<uint32_t>& selectedIndex, Node*& pSelectedNode ) const IFNOEXCEPT
{
	const auto node_flags =
		( selectedIndex.has_value() && index == *selectedIndex ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None ) |
		( childPtrs.empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow );

	const bool isExpanded = ImGui::TreeNodeEx( reinterpret_cast<void*>( (intptr_t)index ), node_flags, name.c_str() );
	if( ImGui::IsItemClicked() )
	{
		selectedIndex = index;
		pSelectedNode = const_cast<Node*>( this );
	}

	if( isExpanded )
	{
		for( const auto& pc : childPtrs )
		{
			pc->ShowTree( selectedIndex, pSelectedNode );
		}
		ImGui::TreePop();
	}
}

Model::Model( Graphics& gfx, std::string filename )
{
	Assimp::Importer importer;
	auto pModel = importer.ReadFile(
		filename,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals
	);

	meshPtrs.reserve( pModel->mNumMeshes );
	for( size_t i = 0; i < pModel->mNumMeshes; i++ )
	{
		meshPtrs.push_back( ParseMesh( gfx, *pModel->mMeshes[i] ) );
	}

	pRoot = ParseNode( *pModel->mRootNode );
}

void Model::Draw( Graphics & gfx ) const noexcept( !IS_DEBUG )
{
	if( auto node = pModelWindow->GetSelectedNode() )
	{
		node->SetAppliedTransform( pModelWindow->GetTransform() );
	}
	pRoot->Draw( gfx, dx::XMMatrixIdentity() );
}

void Model::ShowWindow( const char * name ) const noexcept( !IS_DEBUG )
{
	pModelWindow->ShowWindow( name, *pRoot );
}

Model::~Model() noexcept = default;

std::unique_ptr<Mesh> Model::ParseMesh( Graphics & gfx, const aiMesh & mesh ) IFNOEXCEPT
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
			*reinterpret_cast<dx::XMFLOAT3*>( &mesh.mVertices[i] ), // mVertices is vertex positions
			*reinterpret_cast<dx::XMFLOAT3*>( &mesh.mNormals[i] )
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
		dx::XMFLOAT3 color = { 0.6f, 0.6f, 0.8f };
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	} pMc;

	bindablePtrs.push_back( std::make_unique<PixelConstantBuffer<PSMaterialConstant>>( gfx, pMc, 1u ) );

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

	static uint32_t i = 0;

	auto pNode = std::make_unique<Node>( std::move( curMeshPtrs ), node.mName.C_Str(), i++, parentTransform );

	for( uint32_t i = 0; i < node.mNumChildren; i++ )
	{
		pNode->AddChild( ParseNode( *node.mChildren[i] ) );
	}

	return pNode;
}