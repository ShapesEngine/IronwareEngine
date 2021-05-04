/*!
 * \file Node.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "Node.h"
#include "Mesh.h"
#include "FrameExecutor.h"
#include "Model.h"
#include "ModelWindow.h"

Node::Node( std::vector<Mesh*> meshPtrs, const std::string & name, uint32_t index, const DirectX::XMMATRIX & transform_in ) noexcept( !IS_DEBUG ) :
	meshPtrs( std::move( meshPtrs ) ),
	name( name ),
	index( index )
{
	dx::XMStoreFloat4x4( &parentTransform, transform_in );
	dx::XMStoreFloat4x4( &appliedTransform, dx::XMMatrixIdentity() );
}

void Node::Submit( FrameExecutor& frame, dx::FXMMATRIX accumulatedTransform ) const IFNOEXCEPT
{
	const auto built =
		dx::XMLoadFloat4x4( &appliedTransform ) *
		dx::XMLoadFloat4x4( &parentTransform ) *
		accumulatedTransform;
	for( const auto pm : meshPtrs )
	{
		pm->Submit( frame, built );
	}

	for( const auto& pc : childPtrs )
	{
		pc->Submit( frame, built );
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