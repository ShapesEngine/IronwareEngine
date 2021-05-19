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
#include "Model.h"
#include "ModelProbe.h"

Node::Node( std::vector<Mesh*> meshPtrs, const std::string & name, uint32_t index, const DirectX::XMMATRIX & transform_in ) noexcept( !IS_DEBUG ) :
	meshPtrs( std::move( meshPtrs ) ),
	name( name ),
	index( index )
{
	dx::XMStoreFloat4x4( &parentTransform, transform_in );
	dx::XMStoreFloat4x4( &appliedTransform, dx::XMMatrixIdentity() );
}

void Node::Submit( size_t channelFilter, dx::FXMMATRIX accumulatedTransform ) const IFNOEXCEPT
{
	const auto built =
		dx::XMLoadFloat4x4( &appliedTransform ) *
		dx::XMLoadFloat4x4( &parentTransform ) *
		accumulatedTransform;
	for( const auto pm : meshPtrs )
	{
		pm->Submit( channelFilter, built );
	}

	for( const auto& pc : childPtrs )
	{
		pc->Submit( channelFilter, built );
	}
}

void Node::Accept( ModelProbe & probe )
{
	if( probe.PushNode( *this ) )
	{
		for( auto& cp : childPtrs )
		{
			cp->Accept( probe );
		}
		probe.PopNode( *this );
	}
}

void Node::Accept( TechniqueProbe& probe )
{
	for( auto& mp : meshPtrs )
	{
		mp->Accept( probe );
	}
}

void Node::AddChild( std::unique_ptr<Node> pChild ) IFNOEXCEPT
{
	assert( pChild );
	childPtrs.push_back( std::move( pChild ) );
}