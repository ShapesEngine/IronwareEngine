/*!
 * \file IndexedTriangleList.h
 * \date 2020/09/25 21:15
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include <vector>

#include <DirectXMath.h>

/*!
 * \class IndexedTriangleList
 *
 * \brief A class that stores object's vertices & indices.
 * * Also can perform various transformations on that vertices.
 *
 * \author Yernar Aldabergenov
 * \date March 2020
 */
template<class T>
class IndexedTriangleList
{
public:
	IndexedTriangleList() = default;
	IndexedTriangleList( std::vector<T> verts_in, std::vector<uint16_t> indices_in ) :
		vertices( std::move( verts_in ) ),
		indices( std::move( indices_in ) )
	{
		assert( vertices.size() > 2 );
		assert( indices.size() % 3 == 0 );
	}

	void Transform( DirectX::FXMMATRIX matrix )
	{
		for( auto& v : vertices )
		{
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3( &v.pos );
			DirectX::XMStoreFloat3(
				&v.pos,
				DirectX::XMVector3Transform( pos, matrix )
			);
		}
	}

	// asserts face-independent vertices w/ normals cleared to zero
	void SetNormalsIndependentFlat() noexcept( !IS_DEBUG )
	{
		using namespace DirectX;
		assert( indices.size() % 3 == 0 && !indices.empty() );
		for( size_t i = 0; i < indices.size(); i += 3 )
		{
			auto& v0 = vertices[indices[i]];
			auto& v1 = vertices[indices[i + 1]];
			auto& v2 = vertices[indices[i + 2]];
			const auto p0 = XMLoadFloat3( &v0.pos );
			const auto p1 = XMLoadFloat3( &v1.pos );
			const auto p2 = XMLoadFloat3( &v2.pos );

			const auto n = XMVector3Normalize( XMVector3Cross( ( p1 - p0 ), ( p2 - p0 ) ) );

			XMStoreFloat3( &v0.n, n );
			XMStoreFloat3( &v1.n, n );
			XMStoreFloat3( &v2.n, n );
		}
	}

public:
	std::vector<T> vertices;
	std::vector<uint16_t> indices;
};