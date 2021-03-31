/*!
 * \file Vertex.cpp
 *
 * \author Yernar Aldabergenov
 * \date March 2021
 *
 * 
 */
#include "Vertex.h"

constexpr size_t VertexLayout::Element::SizeOf( Type type ) noexcept( !IS_DEBUG )
{
	using namespace DirectX;
	switch( type )
	{
	case Position2D:
	case Texture2D:
		return sizeof( XMFLOAT2 );

	case Position3D:
	case Normal:
	case Float3Color:
	case Float4Color:
		return sizeof( XMFLOAT3 );

	case BGRAColor:
		return sizeof( uint32_t );
	}
	assert( "Invalid element type" && false );
	return 0u;
}
