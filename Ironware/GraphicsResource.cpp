/*!
 * \file Bindable.cpp
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 *
 *
 */
#include "GraphicsResource.h"
#ifdef NDEBUG
#include <stdexcept>
#endif

DxgiInfoManager& GraphicsResource::GetInfoManager( Graphics& gfx )
{
#ifndef NDEBUG
	return gfx.infoManager;
#else
	throw std::logic_error( "GetInfoManager is not available in Release configuration!" );
#endif
}