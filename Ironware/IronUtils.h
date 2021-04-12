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
#include <typeinfo>

// default buffer size
constexpr size_t DEFAULT_BUFFER_SIZE = 512;

// reinterprets a variable as wide character pointer
#define WCHREINT_CAST( x ) reinterpret_cast<wchar_t*>( x )
// reinterprets a variable as character pointer
#define CHREINT_CAST( x ) reinterpret_cast<char*>( x )

// reinterprets a variable as constant character pointer
#define CON_CHREINT_CAST( x ) reinterpret_cast<const char*>( x )
// reinterprets a variable as wide constant character pointer
#define CON_WCHREINT_CAST( x ) reinterpret_cast<const wchar_t*>( x )

/**
 * @brief Convert Narrow string to Wide string
 * @param narrow input string
 * @return wide output string
*/
std::wstring to_wide( const std::string& narrow );

/**
 * @brief Convert Wide string to Narrow string
 * @param wide input string
 * @return narrow output string
*/
std::string to_narrow( const std::wstring& wide );

/**
 * @brief Convert Narrow char array to Wide string
 * @param narrow input string
 * @return wide output string
*/
std::wstring to_wide( const char* narrow );

/**
 * @brief Convert Wide char array to Narrow string
 * @param wide input string
 * @return narrow output string
*/
std::string to_narrow( const wchar_t* wide );

#define GET_CLASS_WNAME(class) to_wide( typeid( class ).name() )