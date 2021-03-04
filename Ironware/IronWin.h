/*!
 * \file IronWin.h
 * \date 2020/09/11 16:22
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief The header is used to define & include all the macros/headers that are (not) needed/used
 *
 * 
 *
 * \note try to include this header before other headers as it contains pragma once and could be not imported if the previous
 * * header includes Windows.h
*/

#pragma once

// target Windows 7 or later
// you can remove/change it 
// if you'd like to target other platforms or only win10
#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>

// Needed for the cases when other libraries(gdi+) uses these defines
#ifndef IR_FULL_WIN
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

/************************************************************************/
/*   nominmax & strict are defined even in ir_full_win mode            */
/************************************************************************/
#define NOMINMAX
#define STRICT

#define WIDE2( x ) L##x
#define WIDE1( x ) WIDE2( x )
#define WFILE WIDE1( __FILE__ )

#ifdef _WINDOWS_
#error("This header file already includes Windows. Please include IronWin.h rather than Windows.h")
#else
#include <Windows.h>
#endif // _WINDOWS_