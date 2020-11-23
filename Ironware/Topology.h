/*!
 * \file Topology.h
 * \date 2020/11/23 5:43
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief 
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology( Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type );

	inline void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->IASetPrimitiveTopology( type ); }

protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};

