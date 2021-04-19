/*!
 * \file Sheet.h
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#pragma once

#include "Drawable.h"

 /**
  * @brief Drawable sheet class
 */
class Sheet : public Drawable
{
public:
	Sheet( Graphics& gfx, float size );
	void SetPos( const DirectX::XMFLOAT3& pos_in ) noexcept { pos = pos_in; }
	void SetRotation( const DirectX::XMFLOAT3& orientation_in ) noexcept { orientation = orientation_in; }
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	DirectX::XMFLOAT3 pos = { 0.f, 0.f, 0.f };
	DirectX::XMFLOAT3 orientation = { 0.f, 0.f, 0.f };
};