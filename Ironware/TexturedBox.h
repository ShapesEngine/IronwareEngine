/*!
 * \file TexturedBox.h
 *
 * \author Yernar Aldabergenov
 * \date March 2021
 *
 * 
 */
#pragma once

#include "ObjectBase.h"

/*!
 * \class TexturedBox
 *
 * \ingroup Drawables
 *
 * \brief Responsible class for controlling(partly) the graphics pipeline and drawing the textured box
 *
 * \author Yernar Aldabergenov
 *
 * \date March 2021
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class TexturedBox : public ObjectBase<TexturedBox>
{
public:
	TexturedBox( Graphics& gfx, std::mt19937& rng,
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

