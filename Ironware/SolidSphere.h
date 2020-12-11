/*!
 * \file SolidSphere.h
 * \date 2020/12/11 5:14
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
#include "DrawableBase.h"

class SolidSphere : public DrawableBase<SolidSphere>
{
public:
	SolidSphere( Graphics& gfx, float radius );

	inline void Update( float dt ) noexcept override {}
	inline void SetPosition( DirectX::XMFLOAT3 pos ) noexcept { position = pos; }
	inline DirectX::XMMATRIX GetTransformXM() const noexcept override { return DirectX::XMMatrixTranslation( position.x, position.y, position.z ); }

private:
	DirectX::XMFLOAT3 position = { 1.0f, 1.0f, 1.0f };
};