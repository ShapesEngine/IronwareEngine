/*!
 * \file WindowsMessageMap.h
 *
 * \author Magic
 * \date November 2020
 *
 * 
 */

#pragma once
#include <unordered_map>
#include <Windows.h>

class WindowsMessageMap
{
public:
	WindowsMessageMap() noexcept;
	std::string operator()( DWORD msg, LPARAM lp, WPARAM wp ) const noexcept;

private:
	std::unordered_map<DWORD, std::string> map;
};
