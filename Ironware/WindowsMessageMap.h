/*!
 * \file WindowsMessageMap.h
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 *
 * 
 */

#pragma once

#include <unordered_map>
#include "IronWin.h"

class WindowsMessageMap
{
public:
	WindowsMessageMap() noexcept;
	std::wstring operator()( DWORD msg, LPARAM lp, WPARAM wp ) const noexcept;

private:
	std::unordered_map<DWORD, std::wstring> map;
};
