/*!
 * \file Box.h
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#pragma once

#include "Drawable.h"

 /**
  * @brief Drawable box class
 */
class Box : public Drawable
{
public:
	Box( Graphics& gfx, float size );
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SpawnControlWindow( Graphics& gfx, const char* name ) noexcept;
	void SetPos( const DirectX::XMFLOAT3& pos_in ) noexcept { pos = pos_in; }
	void SetRotation( const DirectX::XMFLOAT3& orientation_in ) noexcept { orientation = orientation_in; }

private:
	DirectX::XMFLOAT3 pos = { 0.f, 0.f, 0.f };
	DirectX::XMFLOAT3 orientation = { 0.f, 0.f, 0.f };
};