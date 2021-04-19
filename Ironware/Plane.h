/*!
 * \file Plane.h
 * \date 2020/09/25 22:26
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A header that contains a (geometry) plane class
 *
 * TODO:
 *
 * \note Call Make if you don't want to set division x & y manually
*/
#pragma once

#include "IndexedTriangleList.h"
#include "IronMath.h"

#include <vector>
#include <array>

/*!
 * \class Plane
 *
 * \brief A class for generating plane vertices and indices
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 */
class Plane
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselatedTextured( int divisions_x, int divisions_y )
	{
		namespace dx = DirectX;

		assert( divisions_x >= 1 );
		assert( divisions_y >= 1 );

		constexpr float width = 2.f;
		constexpr float height = 2.f;
		const int nVertices_x = divisions_x + 1;
		const int nVertices_y = divisions_y + 1;
		std::vector<V> vertices( (size_t)nVertices_x * nVertices_y );

		{
			const float side_x = width / 2.f;
			const float side_y = height / 2.f;
			const float divisionSize_x = width / float( divisions_x );
			const float divisionSize_y = height / float( divisions_y );
			const float divisionSize_tex_x = 1.f / float( divisions_x );
			const float divisionSize_tex_y = 1.f / float( divisions_y );
			const auto bottomLeft = dx::XMVectorSet( -side_x, -side_y, 0.f, 0.f );

			for( int y = 0, i = 0; y < nVertices_y; y++ )
			{
				const float y_pos = float( y ) * divisionSize_y;
				const float y_tex_pos = float( y ) * divisionSize_tex_y;
				for( int x = 0; x < nVertices_x; x++, i++ )
				{
					const auto v = dx::XMVectorAdd(
						bottomLeft,
						dx::XMVectorSet( float( x ) * divisionSize_x, y_pos, 0.f, 0.f )
					);
					const auto x_tex_pos = float( x ) * divisionSize_tex_x;

					dx::XMStoreFloat3( &vertices[i].pos, v );
					vertices[i].n = { 0.f, 0.f, -1.f };
					vertices[i].tex = { x_tex_pos, y_tex_pos };
				}
			}
		}

		std::vector<uint16_t> indices;
		indices.reserve( (size_t)sq( divisions_x * divisions_y ) * 6 );
		{
			// vertex x and y to index
			const auto vxy2i = [nVertices_x]( size_t x, size_t y )
			{
				return (uint16_t)( y * nVertices_x + x );
			};
			for( size_t y = 0; y < divisions_y; y++ )
			{
				for( size_t x = 0; x < divisions_x; x++ )
				{
					const std::array<uint16_t, 4> indexArray =
					{ vxy2i( x, y ), vxy2i( x + 1, y ), vxy2i( x, y + 1 ), vxy2i( x + 1, y + 1 ) };
					indices.push_back( indexArray[0] );
					indices.push_back( indexArray[2] );
					indices.push_back( indexArray[1] );
					indices.push_back( indexArray[1] );
					indices.push_back( indexArray[2] );
					indices.push_back( indexArray[3] );
				}
			}
		}

		return{ std::move( vertices ), std::move( indices ) };
	}

	template<class V>
	static IndexedTriangleList<V> Make() { return MakeTesselatedTextured<V>( 1, 1 ); }
};