/*!
 * \file Cone.h
 * \date 2020/09/25 22:37
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A Pyramid child class will control(partly) the graphics pipeline and draw the cone
 *
 * \note It contains variables that are needed for translation, rotation, etc.
*/
#pragma once

#include "ObjectBase.h"

/*!
 * \class Cone
 *
 * \ingroup Drawables
 *
 * \brief Responsible class for controlling(partly) the graphics pipeline and drawing the cone
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class Cone : public ObjectBase<Cone>
{
public:
	Cone( Graphics& gfx, std::mt19937& rng,
			 std::uniform_real_distribution<float>& adist,
			 std::uniform_real_distribution<float>& ddist,
			 std::uniform_real_distribution<float>& odist,
			 std::uniform_real_distribution<float>& rdist,
			 std::uniform_int_distribution<>& tdist );
};