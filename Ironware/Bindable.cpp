/*!
 * \file Bindable.cpp
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 *
 *
 */
#include "Bindable.h"
#ifdef NDEBUG
#include <stdexcept>
#endif

DxgiInfoManager& Bindable::GetInfoManager( Graphics& gfx )
{
#ifndef NDEBUG
	return gfx.infoManager;
#else
	throw std::logic_error( "GetInfoManager is not available in Release configuration!" );
#endif
}
