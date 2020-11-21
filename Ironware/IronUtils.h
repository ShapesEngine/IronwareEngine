/*!
 * \file IronUtils.h
 * \date 2020/11/16 6:54
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief Some utils
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include <string>

constexpr size_t DEFAULT_BUFFER_SIZE = 512;

#define WCHREINT_CAST( x ) reinterpret_cast<wchar_t*>( x )
#define CHREINT_CAST( x ) reinterpret_cast<char*>( x )

#define CON_CHREINT_CAST( x ) reinterpret_cast<const char*>( x )
#define CON_WCHREINT_CAST( x ) reinterpret_cast<const wchar_t*>( x )

std::wstring ToWide( const std::string& narrow );

std::string ToNarrow( const std::wstring& wide );


