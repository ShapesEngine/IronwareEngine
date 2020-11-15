/*!
 * \file DxgiInfoManager.h
 * \date 2020/11/16 4:38
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A helper class that is used for getting DXGI error information
 *
 * TODO:
 *
 * \note Call Set every time you want to capture an error. Call => DirectXFunc => GetMessages
*/
#pragma once

#include "IronWin.h"

#include <vector>
#include <string>

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	DxgiInfoManager( const DxgiInfoManager& ) = delete;
	DxgiInfoManager& operator=( const DxgiInfoManager& ) = delete;
	~DxgiInfoManager();
	
	void Set() noexcept;
	std::vector<std::wstring> GetMessages() const;

private:
	unsigned long long next = 0u;
	struct IDXGIInfoQueue* pDxgiInfoQueue = nullptr;
};