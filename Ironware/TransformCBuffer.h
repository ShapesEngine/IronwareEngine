/*!
 * \file TransformCBuffer.h
 * \date 2020/11/23 5:46
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A Transform Constant Buffer wrapper child class that will be bound in process of graphics pipeline.
 *
 * TODO:
 *
 * \note It stores parent and vertexConstBuffer. parent is needed to get transform matrix and
 * vertConstBuffer is needed to update and bind the buffer.
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
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVertConstBuffer;
	const Drawable& parent;
};

