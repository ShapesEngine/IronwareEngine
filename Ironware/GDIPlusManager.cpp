/*!
 * \file GDIPlusManager.cpp
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 *
 * 
 */
#include "GDIPlusStd.inl"
#include "GDIPlusManager.h"

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
