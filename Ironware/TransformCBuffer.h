/*!
 * \file TransformCBuffer.h
 * \date 2020/11/23 5:46
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief 
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "Bindable.h"
#include "Drawable.h"
#include "ConstantBuffers.h"

#include <DirectXMath.h>

class TransformCBuffer : public Bindable
{
public:
	TransformCBuffer( Graphics& gfx, const Drawable& parent );
	inline void Bind( Graphics& gfx ) noexcept override;

private:
	VertexConstantBuffer<DirectX::XMMATRIX> vcbuf;
	const Drawable& parent;
};

