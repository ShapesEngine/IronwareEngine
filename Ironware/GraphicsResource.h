/*!
 * \file GraphicsResource.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "Graphics.h"

class GraphicsResource
{
protected:
	static ID3D11DeviceContext* GetContext( Graphics& gfx ) noexcept { return gfx.pImmediateContext.Get(); }
	static ID3D11Device* GetDevice( Graphics& gfx ) noexcept { return gfx.pDevice.Get(); }

	/**
	 * @brief Avoid calling this function directly, instead call INFOMAN macro
	 * * that will automatically get the infomanager
	 * @param gfx Graphics object where the infomanager is stored
	 * @return Reference to the DXgiInfoManager object from graphics instance
	*/
	static DxgiInfoManager& GetInfoManager( Graphics& gfx );
};