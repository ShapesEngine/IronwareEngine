/*!
 * \file IronUtils.h
 * \date 2020/09/16 6:54
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief Collection of utility functions
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include <string>

// default buffer size
constexpr size_t DEFAULT_BUFFER_SIZE = 512;

// wide char reinterpret cast
#define WCHREINT_CAST( x ) reinterpret_cast<wchar_t*>( x )
// char reinterpret cast
#define CHREINT_CAST( x ) reinterpret_cast<char*>( x )

// const char reinterpret cast
#define CON_CHREINT_CAST( x ) reinterpret_cast<const char*>( x )
// const wide char reinterpret cast
#define CON_WCHREINT_CAST( x ) reinterpret_cast<const wchar_t*>( x )

/**
 * @brief Convert Narrow string to Wide string
 * @param narrow input string 
 * @return wide output string
*/
std::wstring ToWide( const std::string& narrow );

/**
 * @brief Convert Wide string to Narrow string
 * @param wide input string
 * @return narrow output string
*/
std::string ToNarrow( const std::wstring& wide );


