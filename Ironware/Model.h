/*!
 * \file Model.h
 *
 * \author Yernar Aldabergenov
 * \date March 2021
 *
 * 
 */
#pragma once

#include "ObjectBase.h"

/*!
 * \class Model
 *
 * \ingroup Drawables
 *
 * \brief Responsible class for drawing a mesh from specified file
 *
 * \author Yernar Aldabergenov
 *
 * \date March 2021
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class Model : public ObjectBase<Model>
{
public:
	Model( Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		DirectX::XMFLOAT3 material,
		float scale );
};