/*!
 * \file PrimitiveTopology.h
 * \date 2020/09/23 5:43
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A PrimitiveTopology wrapper child class that will be bound in process of graphics pipeline.
 *
 * TODO:
 *
 * \note It stores primitive PrimitiveTopology type
*/
#pragma once

#include "Bindable.h"

class PrimitiveTopology : public Bindable
{
public:
	PrimitiveTopology( Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type );

	__forceinline void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->IASetPrimitiveTopology( type ); }

protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};

