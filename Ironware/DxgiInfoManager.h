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
 * \note Call Set every time you want to capture an error. Call Set => DirectXFunc => GetMessages
 * * Set moves the message index to the present so that it will show only the messages related to the
 * * function call
*/
#pragma once

#include "IronWin.h"

#include <dxgidebug.h>
#include <vector>
#include <string>
#include <wrl.h>

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	DxgiInfoManager( const DxgiInfoManager& ) = delete;
	DxgiInfoManager& operator=( const DxgiInfoManager& ) = delete;
	~DxgiInfoManager() = default;
	
	void Set() noexcept;
	std::vector<std::wstring> GetMessages() const;

private:
	unsigned long long next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
};