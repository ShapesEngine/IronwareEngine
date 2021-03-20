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

private:
	// model transform
	DirectX::XMFLOAT3X3 mt = {};
};