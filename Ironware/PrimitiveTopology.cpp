/*!
 * \class PrimitiveTopology
 *
 * \brief A PrimitiveTopology wrapper child class that will be bound in process of graphics pipeline.
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 */
#include "PrimitiveTopology.h"

PrimitiveTopology::PrimitiveTopology( Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type ) :
	type( type )
{}
