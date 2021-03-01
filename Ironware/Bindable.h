/*!
 * \file Bindable.h
 * \date 2020/09/23 4:31
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief An abstract class that will be inherited from other bindable types.
 * * All the child classes will get bound with the Bind function.
 *
 *
 * TODO:
 *
 * \note The reason that static functions exists is because Bindable is friend of Graphics class and we would
 * * like to give access only to these members for child classes
*/
#pragma once

#include "Graphics.h"
#include "DxgiInfoManager.h"

class Bindable
{
public:
	virtual void Bind( Graphics& gfx ) noexcept = 0;
	virtual ~Bindable() = default;

protected:
	__forceinline static ID3D11DeviceContext* GetContext( Graphics& gfx ) noexcept { return gfx.pImmediateContext.Get(); }
	__forceinline static ID3D11Device* GetDevice( Graphics& gfx ) noexcept { return gfx.pDevice.Get(); }
	static DxgiInfoManager& GetInfoManager( Graphics& gfx ) noexcept( IS_DEBUG );
};

