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
	void DrawOutline( Graphics& gfx ) IFNOEXCEPT;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SpawnControlWindow( Graphics& gfx, const char* name ) noexcept;
	void Reset() noexcept;
	void SetPos( const DirectX::XMFLOAT3& pos_in ) noexcept { pos = pos_in; }
	void SetRotation( const DirectX::XMFLOAT3& orientation_in ) noexcept { orientation = orientation_in; }

private:
	std::vector<std::shared_ptr<Bindable>> outlineBindables;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 orientation;
	struct SheetCBuff
	{
		float specularIntensity;
		float specularPower;
		alignas( 8 ) BOOL isNormalMappingEnabled;
	}cbuff;
	bool isOutlineEnabled = false;
};