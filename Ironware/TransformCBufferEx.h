/*!
 * \file TransformCBufferEx.h
 *
 * \author Yernar Aldabergenov
 * \date April 2021
 *
 *
 */
#pragma once

#include "TransformCBuffer.h"

 /**
  * @brief Extended version of bindable transform constant buffer class
  * * which has vertex and pixel constant buffers
 */
class TransformCBufferEx : public TransformCBuffer
{
public:
	TransformCBufferEx( Graphics& gfx, const Drawable& parent, UINT slotVS = 0u, UINT slotPS = 0u );
	void Bind( Graphics& gfx ) noexcept override;

protected:
	void UpdateBind( Graphics& gfx, const Transforms& transforms ) noexcept;

private:
	static std::unique_ptr<PixelConstantBuffer<Transforms>> pPixConstBuffer;
};