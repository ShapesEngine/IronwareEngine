/*!
 * \class GDIPlusManager
 *
 * \brief 
 *
 * \author Yernar Aldabergenov
 * \date December 2020
 */
#define FULL_WIN
#include "IronWin.h"
#include "GDIPlusManager.h"

#include <algorithm>
namespace Gdiplus
{
	// replacing the min max from windows with stdminmax
	using std::min;
	using std::max;
}
#include <gdiplus.h>

ULONG_PTR GDIPlusManager::token = 0;
int GDIPlusManager::refCount = 0;

GDIPlusManager::GDIPlusManager()
{
	if( refCount++ == 0 )
	{
		Gdiplus::GdiplusStartupInput input;
		input.GdiplusVersion = 1;
		input.DebugEventCallback = nullptr;
		input.SuppressBackgroundThread = false;
		Gdiplus::GdiplusStartup( &token, &input, nullptr );
	}
}

GDIPlusManager::~GDIPlusManager()
{
	if( --refCount == 0 )
	{
		Gdiplus::GdiplusShutdown( token );
	}
}
