/*!
 * \class Cylinder
 *
 * \brief A header that contains a (drawable) cylinder class
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
#pragma once

#include "ObjectBase.h"

/*!
 * \class Cylinder
 *
 * \ingroup Drawables
 *
 * \brief Responsible class for controlling(partly) the graphics pipeline and drawing the cylinder
 *
 * \author Yernar Aldabergenov
 *
 * \date November 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class Cylinder : public ObjectBase<Cylinder>
{
public:
	Cylinder( Graphics& gfx, std::mt19937& rng,
			  std::uniform_real_distribution<float>& adist,
			  std::uniform_real_distribution<float>& ddist,
			  std::uniform_real_distribution<float>& odist,
			  std::uniform_real_distribution<float>& rdist,
			  std::uniform_real_distribution<float>& bdist,
			  std::uniform_int_distribution<int>& tdist );
};