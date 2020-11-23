/*!
 * \class Bindable
 *
 * \brief 
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
#include "Bindable.h"

DxgiInfoManager& Bindable::GetInfoManager( Graphics& gfx ) noexcept( !IS_DEBUG )
{
#ifndef NDEBUG
	return gfx.infoManager;
#else
	throw std::logic_error( "GetInfoManager is not available in Release configuration!" );
#endif
}
