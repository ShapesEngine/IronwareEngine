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
	Sheet( Graphics& gfx, float size, DirectX::XMFLOAT4 color );
	void SetPos( const DirectX::XMFLOAT3& pos_in ) noexcept { pos = pos_in; }
	void SetRotation( const DirectX::XMFLOAT3& orientation_in ) noexcept { orientation = orientation_in; }
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SpawnControlWindow( Graphics& gfx, const char* pName = nullptr ) noexcept;
	void Reset() noexcept;

private:
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 orientation;
	struct SheetCBuff
	{
		DirectX::XMFLOAT4 color;
	}cbuff;
};