/*!
 * \file Cube.h
 * \date 2020/11/25 21:41
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A class for generating cube vertices and indices
 *
 * TODO:
 *
 * \note Skinned Cube needs to have separate vertices
*/
#pragma once

#include "IndexedTriangleList.h"

#include <DirectXMath.h>

class Cube
{
public:
	template<class V>
	static IndexedTriangleList<V> Make()
	{
		namespace dx = DirectX;

		// ndc; width and height will be 1.f
		constexpr float side = 1.f / 2.f;

		std::vector<V> vertices( 8 );
		vertices[0].pos = { -side, -side, -side };
		vertices[1].pos = { side, -side, -side };
		vertices[2].pos = { -side, side, -side };
		vertices[3].pos = { side, side, -side };
		vertices[4].pos = { -side, -side, side };
		vertices[5].pos = { side, -side, side };
		vertices[6].pos = { -side, side, side };
		vertices[7].pos = { side, side, side };

		return {
			std::move( vertices ), {
				0, 2, 1, 2, 3, 1,
				1, 3, 5, 3, 7, 5,
				2, 6, 3, 3, 6, 7,
				4, 5, 7, 4, 7, 6,
				0, 4, 2, 2, 4, 6,
				0, 1, 4, 1, 5, 4
			}
		};
	}

	template<class V>
	static IndexedTriangleList<V> MakeSkinned()
	{
		namespace dx = DirectX;

		constexpr float side = 1.f / 2.f;

		std::vector<V> vertices( 14 );

		vertices[0].pos = { -side, -side, -side };
		vertices[0].tex = { 2.f / 3.f, 0.f / 4.f };
		vertices[1].pos = { side, -side, -side };
		vertices[1].tex = { 1.f / 3.f, 0.f / 4.f };
		vertices[2].pos = { -side, side, -side };
		vertices[2].tex = { 2.f / 3.f, 1.f / 4.f };
		vertices[3].pos = { side, side, -side };
		vertices[3].tex = { 1.f / 3.f, 1.f / 4.f };
		vertices[4].pos = { -side, -side, side };
		vertices[4].tex = { 2.f / 3.f, 3.f / 4.f };
		vertices[5].pos = { side, -side, side };
		vertices[5].tex = { 1.f / 3.f, 3.f / 4.f };
		vertices[6].pos = { -side, side, side };
		vertices[6].tex = { 2.f / 3.f, 2.f / 4.f };
		vertices[7].pos = { side, side, side };
		vertices[7].tex = { 1.f / 3.f, 2.f / 4.f };
		vertices[8].pos = { -side, -side, -side };
		vertices[8].tex = { 2.f / 3.f, 4.f / 4.f };
		vertices[9].pos = { side, -side, -side };
		vertices[9].tex = { 1.f / 3.f, 4.f / 4.f };
		vertices[10].pos = { -side, -side, -side };
		vertices[10].tex = { 3.f / 3.f, 1.f / 4.f };
		vertices[11].pos = { -side, -side, side };
		vertices[11].tex = { 3.f / 3.f, 2.f / 4.f };
		vertices[12].pos = { side, -side, -side };
		vertices[12].tex = { 0.f / 3.f, 1.f / 4.f };
		vertices[13].pos = { side, -side, side };
		vertices[13].tex = { 0.f / 3.f, 2.f / 4.f };

		return{
			std::move( vertices ), {
				0, 2, 1, 2, 3, 1,
				4, 8, 5, 5, 8, 9,
				2, 6, 3, 3, 6, 7,
				4, 5, 7, 4, 7, 6,
				2, 10, 11, 2, 11, 6,
				12, 3, 7, 12, 7, 13
		}
		};
	}

	template<class V>
	static IndexedTriangleList<V> MakeIndependent()
	{
		constexpr float side = 1.0f / 2.0f;

		std::vector<V> vertices( 24 );
		/******************************* near side ******************************/
		vertices[0].pos = { -side, -side, -side };// 0
		vertices[1].pos = { side, -side, -side };// 1
		vertices[2].pos = { -side, side, -side };// 2
		vertices[3].pos = { side, side, -side };// 3
		/******************************* far side ******************************/
		vertices[4].pos = { -side, -side, side };// 4
		vertices[5].pos = { side, -side, side };// 5
		vertices[6].pos = { -side, side, side };// 6
		vertices[7].pos = { side, side, side };// 7
		/******************************* left side ******************************/
		vertices[8].pos = { -side, -side, -side };// 8
		vertices[9].pos = { -side, side, -side };// 9
		vertices[10].pos = { -side, -side, side };// 10
		vertices[11].pos = { -side, side, side };// 11
		/******************************* right side ******************************/
		vertices[12].pos = { side, -side, -side };// 12
		vertices[13].pos = { side, side, -side };// 13
		vertices[14].pos = { side, -side, side };// 14
		vertices[15].pos = { side, side, side };// 15
		/******************************* bottom side ******************************/
		vertices[16].pos = { -side, -side, -side };// 16
		vertices[17].pos = { side, -side, -side };// 17
		vertices[18].pos = { -side, -side, side };// 18
		vertices[19].pos = { side, -side, side };// 19
		/******************************* top side ******************************/
		vertices[20].pos = { -side, side, -side };// 20
		vertices[21].pos = { side, side, -side };// 21
		vertices[22].pos = { -side, side, side };// 22
		vertices[23].pos = { side, side, side };// 23

		return{
			std::move( vertices ), {
				0, 2, 1, 2, 3, 1,
				4, 5, 7, 4, 7, 6,
				8, 10, 9, 10, 11, 9,
				12, 13, 15, 12, 15, 14,
				16, 17, 18, 18, 17, 19,
				20, 23, 21, 20, 22, 23
			}
		};
	}
};
