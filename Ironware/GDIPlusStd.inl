/*!
 * \file GDIPlusStd.inl
 *
 * \author Yernar Aldabergenov
 * \date March 2021
 *
 * 
 */
#pragma once

#define IR_FULL_WIN
#include "IronWin.h"

#include <algorithm>
namespace Gdiplus
{
	// replacing the min max from windows with stdminmax
	using std::min;
	using std::max;
}
#include <gdiplus.h>