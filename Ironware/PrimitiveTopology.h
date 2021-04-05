/*!
 * \file PrimitiveTopology.h
 * \date 2020/09/23 5:43
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A header that contains a (bindable) primitivetopology class
 *
*/
#pragma once

#include "Bindable.h"

/*!
 * \class PrimitiveTopology
 *
 * \ingroup Bindables
 *
 * \brief A PrimitiveTopology wrapper child class that will be bound in process of graphics pipeline.
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class PrimitiveTopology : public Bindable
{
public:
	PrimitiveTopology( Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->IASetPrimitiveTopology( type ); }

protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};