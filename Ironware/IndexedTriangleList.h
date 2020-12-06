/*!
 * \file IndexedTriangleList.h
 * \date 2020/11/25 21:15
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A class for generating indices and vertices.
 * * Also can translate the vertices
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include <vector>

#include <DirectXMath.h>

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

public:
	std::vector<T> vertices;
	std::vector<uint16_t> indices;
};