/*!
 * \file Graphics.h
 * \date 2020/11/15 20:57
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A class that's responsible for the graphical output
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "IronWin.h"
#include <d3d11.h>

class Graphics
{
public:
	Graphics( HWND hWnd );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	~Graphics();

private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;	
	ID3D11DeviceContext* pImmediateContext = nullptr;
};

