/*!
 * \file IronUtils.cpp
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
#include "IronUtils.h"

std::wstring to_wide( const std::string& narrow )
{
	wchar_t wide[DEFAULT_BUFFER_SIZE];
	mbstowcs_s( nullptr, wide, narrow.c_str(), _TRUNCATE );
	return wide;
}

std::string to_narrow( const std::wstring& wide )
{
	char narrow[DEFAULT_BUFFER_SIZE];
	wcstombs_s( nullptr, narrow, wide.c_str(), _TRUNCATE );
	// terminating with 0 as if the size is > 512 it wont have 0 terminator
	narrow[DEFAULT_BUFFER_SIZE - 1] = '\0';
	return narrow;
}

std::wstring to_wide( const char * narrow )
{
	wchar_t wide[DEFAULT_BUFFER_SIZE];
	mbstowcs_s( nullptr, wide, narrow, _TRUNCATE );
	return wide;
}

std::string to_narrow( const wchar_t * wide )
{
	char narrow[DEFAULT_BUFFER_SIZE];
	wcstombs_s( nullptr, narrow, wide, _TRUNCATE );
	// terminating with 0 as if the size is > 512 it wont have 0 terminator
	narrow[DEFAULT_BUFFER_SIZE - 1] = '\0';
	return narrow;
}

std::vector<std::string> split_string( const std::string & s, const std::string & delim )
{
	std::vector<std::string> strings;
	split_string_iter( s, delim, std::back_inserter( strings ) );
	return strings;
}
