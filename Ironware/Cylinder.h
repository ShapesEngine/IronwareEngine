/*!
 * \class Cylinder
 *
 * \brief 
 *
 * \author Yernar Aldabergenov
 * \date December 2020
 */
#pragma once

#include "ObjectBase.h"

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

