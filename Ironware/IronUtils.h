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

std::wstring ToWide( const std::string& narrow );

std::string ToNarrow( const std::wstring& wide );


