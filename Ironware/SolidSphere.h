/*!
 * \file SolidSphere.h
 * \date 2020/11/11 5:14
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A class that is responsible for creating & updating the sphere 
 *
*/
#pragma once

#include "DrawableBase.h"

/*!
 * \class SolidSphere
 *
 * \ingroup Drawables
 *
 * \brief Responsible class for controlling(partly) the graphics pipeline and drawing the solidsphere
 *
 * \author Yernar Aldabergenov
 *
 * \date November 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class SolidSphere : public DrawableBase<SolidSphere>
{
public:
	SolidSphere( Graphics& gfx, float radius );

	void Update( float dt ) noexcept override {}
	void SetPosition( DirectX::XMFLOAT3 pos ) noexcept { position = pos; }
	DirectX::XMMATRIX GetTransformXM() const noexcept override { return DirectX::XMMatrixTranslation( position.x, position.y, position.z ); }

private:
	DirectX::XMFLOAT3 position = { 1.f, 1.f, 1.f };
};