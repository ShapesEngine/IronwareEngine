/*!
 * \file Sphere.h
 * \date 2020/09/25 22:01
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief
 *
 * TODO:
 *
 * \note Call Make if you don't want to set manually lattitude & longitude divisions
*/
#pragma once

#include "IndexedTriangleList.h"
#include "IronMath.h"

#include <DirectXMath.h>

/*!
 * \class Sphere
 *
 * \brief A class for generating sphere vertices and indices
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 */
class Sphere
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated( int latDiv, int longDiv )
	{
		namespace dx = DirectX;
		assert( latDiv >= 3 );
		assert( longDiv >= 3 );

		constexpr float radius = 1.f;
		const auto base = dx::XMVectorSet( 0.f, 0.f, radius, 0.f );
		const float lattitudeAngle = PI / latDiv;
		const float longitudeAngle = 2.f * PI / longDiv;

		std::vector<V> vertices;
		for( int iLat = 1; iLat < latDiv; iLat++ )
		{
			const auto latBase = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationX( lattitudeAngle * iLat )
			);
			for( int iLong = 0; iLong < longDiv; iLong++ )
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					latBase,
					dx::XMMatrixRotationZ( longitudeAngle * iLong )
				);
				dx::XMStoreFloat3( &vertices.back().pos, v );
			}
		}

		// =======================================================================
		// add the cap vertices
		// -----------------------------------------------------------------------
		const auto iNorthPole = (uint16_t)vertices.size();
		vertices.emplace_back();
		dx::XMStoreFloat3( &vertices.back().pos, base );
		const auto iSouthPole = (uint16_t)vertices.size();
		vertices.emplace_back();
		dx::XMStoreFloat3( &vertices.back().pos, dx::XMVectorNegate( base ) );
		// TODO: remove latDiv
		const auto calcIdx = [longDiv]( uint16_t iLong, uint16_t iLat )
		{
			return iLat * longDiv + iLong;
		};
		std::vector<uint16_t> indices;
		// latdiv - 2, because we haven't set 2 rows of the vector
		// we skipped the first & last rows.
		for( uint16_t iLat = 0; iLat < latDiv - 2; iLat++ )
		{
			// skip wrapping
			for( uint16_t iLong = 0; iLong < longDiv - 1; iLong++ )
			{
				indices.emplace_back( calcIdx( iLong, iLat ) );
				indices.emplace_back( calcIdx( iLong, iLat + 1 ) );
				indices.emplace_back( calcIdx( iLong + 1, iLat ) );

				indices.emplace_back( calcIdx( iLong + 1, iLat ) );
				indices.emplace_back( calcIdx( iLong, iLat + 1 ) );
				indices.emplace_back( calcIdx( iLong + 1, iLat + 1 ) );
			}
			// =======================================================================
			// wrap band
			// -----------------------------------------------------------------------
			indices.push_back( calcIdx( longDiv - 1, iLat ) );
			indices.push_back( calcIdx( longDiv - 1, iLat + 1 ) );
			indices.push_back( calcIdx( 0, iLat ) );

			indices.push_back( calcIdx( 0, iLat ) );
			indices.push_back( calcIdx( longDiv - 1, iLat + 1 ) );
			indices.push_back( calcIdx( 0, iLat + 1 ) );
		}

		// =======================================================================
		// cap fans
		// -----------------------------------------------------------------------
		for( uint16_t iLong = 0; iLong < longDiv - 1; iLong++ )
		{
			/******************************* North ******************************/
			indices.push_back( iNorthPole );
			indices.push_back( calcIdx( iLong, 0 ) );
			indices.push_back( calcIdx( iLong + 1, 0 ) );
			/******************************* South ******************************/		
			indices.push_back( calcIdx( iLong + 1, latDiv - 2 ) );
			indices.push_back( calcIdx( iLong, latDiv - 2 ) );
			indices.push_back( iSouthPole );
		}

		// =======================================================================
		// wrap triangles
		// -----------------------------------------------------------------------
		/******************************* North ******************************/
		indices.push_back( iNorthPole );
		indices.push_back( calcIdx( longDiv - 1, 0 ) );
		indices.push_back( calcIdx( 0, 0 ) );
		/******************************* South ******************************/		
		indices.push_back( calcIdx( 0, latDiv - 2 ) );
		indices.push_back( calcIdx( longDiv - 1, latDiv - 2 ) );
		indices.push_back( iSouthPole );

		return { std::move( vertices ), std::move( indices ) };
	}

	template<class V>
	__forceinline static IndexedTriangleList<V> Make() { return MakeTesselated<V>( 12, 24 ); }
};