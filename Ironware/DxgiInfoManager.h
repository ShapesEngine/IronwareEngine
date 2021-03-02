/*!
 * \file DxgiInfoManager.h
 * \date 2020/09/16 4:38
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

/*!
 * \class DxgiInfoManager
 *
 * \ingroup DEV
 *
 * \brief A helper class that is used for getting DXGI error information
 *
 * TODO:
 *
 * \note Call Set every time you want to capture an error. Call Set => DirectXFunc => GetMessages
 * * Set moves the message index to the present so that it will show only the messages related to the
 * * function call
 *
 * \author Yernar Aldabergenov
 *
 * \version 1.0
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
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
	uint64_t next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
};