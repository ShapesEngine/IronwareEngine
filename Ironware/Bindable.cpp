/*!
 * \class Bindable
 *
 * \brief An abstract class that will be inherited from other bindable types.
 * * All the child classes will get bound with the Bind function.
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
#include "Bindable.h"
#ifdef NDEBUG
#include <stdexcept>
#endif

DxgiInfoManager& Bindable::GetInfoManager( Graphics& gfx ) noexcept( IS_DEBUG )
{
#ifndef NDEBUG
	return gfx.infoManager;
#else
	throw std::logic_error( "GetInfoManager is not available in Release configuration!" );
#endif
}
