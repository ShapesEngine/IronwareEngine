/*!
 * \file Prism.h
 * \date 2020/11/25 22:22
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A class for generating prism vertices and indices
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "IndexedTriangleList.h"
#include "IronMath.h"

#include <DirectXMath.h>

class Prism
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated( int longDiv )
	{
		namespace dx = DirectX;

		assert( longDiv >= 3 );

		const auto base = dx::XMVectorSet( 1.f, 0.f, -1.f, 0.f );
		const auto offset = dx::XMVectorSet( 0.f, 0.f, 2.f, 0.f );
		const float longitudeAngle = 2.f * PI / longDiv;

		/******************************* Near Center ******************************/
		std::vector<V> vertices;
		vertices.emplace_back();
		vertices.back().pos = { 0.f, 0.f, -1.f };
		const auto iCenterNear = (uint16_t)( vertices.size() - 1 );
		/******************************* Far Center ******************************/
		vertices.emplace_back();
		vertices.back().pos = { 0.f, 0.f, 1.f };
		const auto iCenterFar = (uint16_t)( vertices.size() - 1 );

		// =======================================================================
		// Base vertices
		// -----------------------------------------------------------------------
		for( int iLong = 0; iLong < longDiv; iLong++ )
		{
			// near base
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationZ( longitudeAngle * iLong )
				);
				dx::XMStoreFloat3( &vertices.back().pos, v );
			}
			// far base
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationZ( longitudeAngle * iLong )
				);
				v = dx::XMVectorAdd( v, offset );
				dx::XMStoreFloat3( &vertices.back().pos, v );
			}
		}

		/******************************* Side indices ******************************/
		std::vector<uint16_t> indices;
		for( uint16_t iLong = 0; iLong < longDiv; iLong++ )
		{
			const auto i = iLong * 2;
			const auto mod = longDiv * 2;
			indices.push_back( i + 2 );
			indices.push_back( ( i + 2 ) % mod + 2 );
			indices.push_back( i + 1 + 2 );
			indices.push_back( ( i + 2 ) % mod + 2 );
			indices.push_back( ( i + 3 ) % mod + 2 );
			indices.push_back( i + 1 + 2 );
		}

		/******************************* Base indicies ******************************/
		for( uint16_t iLong = 0; iLong < longDiv; iLong++ )
		{
			const auto i = iLong * 2;
			const auto mod = longDiv * 2;
			indices.push_back( i + 2 );
			indices.push_back( iCenterNear );
			indices.push_back( ( i + 2 ) % mod + 2 );
			indices.push_back( iCenterFar );
			indices.push_back( i + 1 + 2 );
			indices.push_back( ( i + 3 ) % mod + 2 );
		}

		return { std::move( vertices ), std::move( indices ) };
	}

	template<class V>
	static IndexedTriangleList<V> MakeTesselatedIndependentCapNormals( int longDiv )
	{
		namespace dx = DirectX;
		assert( longDiv >= 3 );

		const auto base = dx::XMVectorSet( 1.f, 0.f, -1.f, 0.f );
		const auto offset = dx::XMVectorSet( 0.f, 0.f, 2.f, 0.f );
		const float longitudeAngle = 2.f * PI / longDiv;

		std::vector<V> vertices;

		// near center
		const auto iCenterNear = (uint16_t)vertices.size();
		vertices.emplace_back();
		vertices.back().pos = { 0.f, 0.f, -1.f };
		vertices.back().n = { 0.f, 0.f, -1.f };
		// near base vertices
		const auto iBaseNear = (uint16_t)vertices.size();
		for( int iLong = 0; iLong < longDiv; iLong++ )
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ( longitudeAngle * iLong )
			);
			dx::XMStoreFloat3( &vertices.back().pos, v );
			vertices.back().n = { 0.f,0.f,-1.f };
		}
		// far center
		const auto iCenterFar = (uint16_t)vertices.size();
		vertices.emplace_back();
		vertices.back().pos = { 0.f, 0.f, 1.f };
		vertices.back().n = { 0.f, 0.f, 1.f };
		// far base vertices
		const auto iBaseFar = (uint16_t)vertices.size();
		for( int iLong = 0; iLong < longDiv; iLong++ )
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ( longitudeAngle * iLong )
			);
			v = dx::XMVectorAdd( v, offset );
			dx::XMStoreFloat3( &vertices.back().pos, v );
			vertices.back().n = { 0.f,0.f,1.f };
		}
		// fusilage vertices
		const auto iFusilage = (uint16_t)vertices.size();
		for( int iLong = 0; iLong < longDiv; iLong++ )
		{
			// near base
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationZ( longitudeAngle * iLong )
				);
				dx::XMStoreFloat3( &vertices.back().pos, v );
				vertices.back().n = { vertices.back().pos.x,vertices.back().pos.y,0.f };
			}
			// far base
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationZ( longitudeAngle * iLong )
				);
				v = dx::XMVectorAdd( v, offset );
				dx::XMStoreFloat3( &vertices.back().pos, v );
				vertices.back().n = { vertices.back().pos.x,vertices.back().pos.y,0.f };
			}
		}

		std::vector<uint16_t> indices;

		/******************************* Near Base Indices ******************************/
		for( uint16_t iLong = 0; iLong < longDiv; iLong++ )
		{
			const auto i = iLong;
			const auto mod = longDiv;
			// near
			indices.push_back( i + iBaseNear );
			indices.push_back( iCenterNear );
			indices.push_back( ( i + 1 ) % mod + iBaseNear );
		}
		/******************************* Far Base Indices ******************************/
		for( uint16_t iLong = 0; iLong < longDiv; iLong++ )
		{
			const auto i = iLong;
			const auto mod = longDiv;
			// far
			indices.push_back( iCenterFar );
			indices.push_back( i + iBaseFar );
			indices.push_back( ( i + 1 ) % mod + iBaseFar );
		}
		// fusilage indices
		for( uint16_t iLong = 0; iLong < longDiv; iLong++ )
		{
			const auto i = iLong * 2;
			const auto mod = longDiv * 2;
			indices.push_back( i + iFusilage );
			indices.push_back( ( i + 2 ) % mod + iFusilage );
			indices.push_back( i + 1 + iFusilage );
			indices.push_back( ( i + 2 ) % mod + iFusilage );
			indices.push_back( ( i + 3 ) % mod + iFusilage );
			indices.push_back( i + 1 + iFusilage );
		}

		return { std::move( vertices ),std::move( indices ) };
	}

	template<class V>
	inline static IndexedTriangleList<V> Make() { return MakeTesselated<V>( 24 ); }
};