/*!
 * \file Box.h
 * \date 2020/09/23 6:17
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A header that contains a (drawable) box class
 *
 * \note It contains variables that are needed for translation, rotation, etc.
*/
#pragma once

#include "ObjectBase.h"
#include "ConstantBuffers.h"

/*!
 * \class Box
 *
 * \ingroup Drawables
 *
 * \brief Responsible class for controlling(partly) the graphics pipeline and drawing the box
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class Box : public ObjectBase<Box>
{
public:
	Box( Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist,
		DirectX::XMFLOAT3 materialColor );

	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SpawnControlWindow( uint32_t id, Graphics& gfx ) noexcept( !IS_DEBUG );

private:
	void SyncMaterials( Graphics& gfx );

private:
	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 color = {};
		float specularIntensity = 0.6f;
		alignas( 16 ) float specularPower = 30.f;
	} materialConstants;

	using MaterialCBuf = PixelConstantBuffer<PSMaterialConstant>;

	// model transform
	DirectX::XMFLOAT3X3 mt = {};
};