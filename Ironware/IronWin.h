/*!
 * \file IronWin.h
 * \date 2020/11/11 16:22
 *
 * \author Magic
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

// target Windows 7 or later
#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>
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
#define NOMINMAX
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

#define STRICT

#define WCHREINT_CAST( x ) reinterpret_cast<wchar_t*>( x )
#define WCON_CHREINT_CAST( x ) reinterpret_cast<const wchar_t*>( x )

#define WIDE2( x ) L##x
#define WIDE1( x ) WIDE2( x )
#define WFILE WIDE1( __FILE__ )

#include <Windows.h>