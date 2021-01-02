/*!
 * \file IronWin.h
 * \date 2020/11/11 16:22
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief The header is used to define all the macros that (not) needed/used
 *
 * 
 *
 * \note try to include this header before other headers as it contains pragma once and could be not imported if the previous
 * * header includes Windows.h
*/

#pragma once

// target Windows 8 or later
// you can remove/change it if 
// you'd like to target other platforms or only win10
#define _WIN32_WINNT 0x0602
#include <sdkddkver.h>

// Needed for the cases when other libraries(gdi+) use these defines
#ifndef FULL_WIN
// The following #defines disable a bunch of unused windows stuff. If you 
// get weird errors when trying to do some windows stuff, try removing some
// (or all) of these defines (it will increase build time though).
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#endif

// minmax is disabled in even full_win mode
#define NOMINMAX
#define STRICT

#define WIDE2( x ) L##x
#define WIDE1( x ) WIDE2( x )
#define WFILE WIDE1( __FILE__ )

// macro that deals with various read access violation errors, mouse, keyboard, etc.
#define SAFE_MESSAGEBOX(hWnd, lpText, lpCaption, uType)	SetWindowLongPtr( GetActiveWindow(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( &DefWindowProc ) ); \
														MessageBox( hWnd, lpText, lpCaption, uType );

#include <Windows.h>