/*!
 * \file TransformCBuffer.h
 * \date 2020/09/23 5:46
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

/*!
 * \class TransformCBuffer
 *
 * \brief A Transform Constant Buffer wrapper child class that will be bound in process of graphics pipeline.
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 */
class TransformCBuffer : public Bindable
{
private:
	struct Transforms
	{
		DirectX::XMMATRIX modelViewProj;
		DirectX::XMMATRIX model;
	};

public:
	TransformCBuffer( Graphics& gfx, const Drawable& parent, UINT slot = 0u );
	void Bind( Graphics& gfx ) noexcept override;

private:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVertConstBuffer;
	const Drawable& parent;
};

