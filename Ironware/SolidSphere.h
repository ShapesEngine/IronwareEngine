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

#include "Drawable.h"
#include "ConstantBuffers.h"

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
class SolidSphere : public Drawable
{
public:
	SolidSphere( Graphics& gfx, float radius );
	void UpdateColor( Graphics& gfx, const DirectX::XMFLOAT3A& col ) noexcept;

	void SetPosition( DirectX::XMFLOAT3 pos ) noexcept { position = pos; }
	DirectX::XMMATRIX GetTransformXM() const noexcept override { return DirectX::XMMatrixTranslation( position.x, position.y, position.z ); }

private:
	// position's proper value is set when the draw call has been called
	DirectX::XMFLOAT3 position;
	// color's proper value is set when the draw call has been called
	DirectX::XMFLOAT3A color;
};