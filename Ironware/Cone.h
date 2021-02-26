/*!
 * \file Cone.h
 * \date 2020/11/25 22:28
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A class for generating cone vertices and indices
 *
 * TODO:
 *
 * \note
*/
#pragma once
#include "IndexedTriangleList.h"
#include "IronMath.h"

#include <DirectXMath.h>

class Cone
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
	__forceinline static IndexedTriangleList<V> Make() { return MakeTesselated<V>( 24 ); }
};