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
	void SpawnControlWindow( Graphics& gfx ) noexcept;
	void Reset() noexcept;

private:
	DirectX::XMFLOAT3 pos = { 0.f, 0.f, 0.f };
	DirectX::XMFLOAT3 orientation = { 0.f, 0.f, 0.f };
	struct SheetCBuff
	{
		float specularIntensity = 0.1f;
		float specularPower = 22.f;
		alignas( 8 ) BOOL isNormalMappingEnabled = TRUE;
	}cbuff;
};