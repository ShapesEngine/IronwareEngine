/*!
 * \file DynamicLayout.h
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#pragma once

#include "DynamicConstantBuffer.h"

#include <string>
#include <memory>
#include <unordered_map>

class DynamicLayout
{
public:
	static CookedLayout Resolve( RawLayout&& layout ) IFNOEXCEPT;

private:
	static DynamicLayout& Get_() noexcept;
	std::unordered_map<std::string, std::shared_ptr<LayoutElement>> map;
};