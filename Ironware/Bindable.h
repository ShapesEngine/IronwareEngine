/*!
 * \file Bindable.h
 * \date 2020/09/23 4:31
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A class that contains a bindable class
 *
 * \note The reason that static functions exists is because Bindable is friend of Graphics class
 * * and we would like to give access only to these members for child classes.
 *
*/
#pragma once

#include "Graphics.h"
#include "DxgiInfoManager.h"

/*!
 * \class Bindable
 *
 * \ingroup Bindables
 *
 * \brief An abstract class that will be inherited from other bindable types.
 * * All the child classes will get bound with the Bind function.
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class Bindable
{
public:
	/**
	 * @brief Binds the bindable type to the pipeline
	 * @param gfx Graphics object where the context and device are stored
	*/
	virtual void Bind( Graphics& gfx ) noexcept = 0;
	virtual ~Bindable() = default;

	virtual std::wstring GenerateUID() const noexcept
	{
		assert( "HMM" );
		return L"";
	}
	virtual std::wstring GetUID() const noexcept
	{
		return GenerateUID();
	}

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