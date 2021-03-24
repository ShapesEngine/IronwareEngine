/*!
 * \file Cone.h
 * \date 2020/09/25 22:28
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A header that contains a (geometry) cone class
 *
 * TODO:
 *
 * \note
*/
#pragma once
#include "IndexedTriangleList.h"
#include "IronMath.h"

#include <DirectXMath.h>

/*!
 * \class Cone
 *
 * \brief A class for generating cone vertices and indices
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 */
class Pyramid
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated( int longDiv )
	{
		namespace dx = DirectX;

		assert( longDiv >= 3 );

		const auto base = dx::XMVectorSet( 1.f, 0.f, -1.f, 0.f );
		const float longitudeAngle = 2.f * PI / longDiv;

		// base vertices
		std::vector<V> vertices;
		for( int iLong = 0; iLong < longDiv; iLong++ )
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ( longitudeAngle * iLong )
			);
			dx::XMStoreFloat3( &vertices.back().pos, v );
		}
		/******************************* Center *****************************/
		vertices.emplace_back();
		vertices.back().pos = { 0.f, 0.f, -1.f };
		const auto iCenter = (uint16_t)( vertices.size() - 1 );
		// -----------------------------------------------------------------------
		/******************************* Tip ******************************/
		vertices.emplace_back();
		vertices.back().pos = { 0.f, 0.f, 1.f };
		const auto iTip = (uint16_t)( vertices.size() - 1 );
		// -----------------------------------------------------------------------

		// base indices
		std::vector<uint16_t> indices;
		for( uint16_t iLong = 0; iLong < longDiv; iLong++ )
		{
			indices.push_back( iCenter );
			indices.push_back( ( iLong + 1 ) % longDiv );
			indices.push_back( iLong );
		}

		// cone indices
		for( uint16_t iLong = 0; iLong < longDiv; iLong++ )
		{
			indices.push_back( iLong );
			indices.push_back( ( iLong + 1 ) % longDiv );
			indices.push_back( iTip );
		}

		return { std::move( vertices ), std::move( indices ) };
	}

	template<class V>
	static IndexedTriangleList<V> MakeTesselatedIndependentFaces( int longDiv )
	{
		namespace dx = DirectX;

		assert( longDiv >= 3 );

		const auto base = dx::XMVectorSet( 1.f, 0.f, -1.f, 0.f );
		const float longitudeAngle = 2.f * PI / longDiv;

		std::vector<V> vertices;

		// cone tip + edge vertices(no center base)
		for( int iLong = 0; iLong < longDiv; iLong++ )
		{
			// tip
			vertices.emplace_back();
			vertices.back().pos = { 0.f, 0.f, 1.f };

			const float angles[] = {
				longitudeAngle * iLong,
				longitudeAngle * ( ( iLong + 1 ) != longDiv ? ( iLong + 1 ) : 0 ),
			};

			for( auto a : angles )
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationZ( a )
				);
				dx::XMStoreFloat3( &vertices.back().pos, v );
			}
		}

		const auto iBaseCenter = (uint32_t)vertices.size();
		vertices.emplace_back();
		vertices.back().pos = { 0.f, 0.f, -1.f };

		const auto iBaseEdges = (uint32_t)vertices.size();
		// basecenter and edge vertices
		for( int iLong = 0; iLong < longDiv; iLong++ )
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ( longitudeAngle * iLong )
			);
			dx::XMStoreFloat3( &vertices.back().pos, v );
		}

		std::vector<uint16_t> indices;

		// cone tip + edge indices
		for( int iLong = 0; iLong < longDiv * 3; iLong++ )
		{
			indices.push_back( iLong );
		}

		// cone base center + edge indices
		for( int iLong = 0; iLong < longDiv; iLong++ )
		{
			indices.push_back( iBaseCenter );
			indices.push_back( ( iLong + 1 ) % longDiv + iBaseEdges );
			indices.push_back( iLong + iBaseEdges );
		}

		return { std::move( vertices ), std::move( indices ) };
	}

	template<class V>
	__forceinline static IndexedTriangleList<V> Make() { return MakeTesselated<V>( 24 ); }
};