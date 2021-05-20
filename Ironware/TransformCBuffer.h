/*!
 * \file TransformCBuffer.h
 * \date 2020/09/23 5:46
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A header that contains a (bindable) transformcbuffer class
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
 * \ingroup Bindables
 *
 * \brief A Transform Constant Buffer wrapper child class that will be bound in process of graphics pipeline.
 *
 * \author Yernar Aldabergenov
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class TransformCBuffer : public CloningBindable
{
protected:
	struct Transforms
	{
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX modelView;
		DirectX::XMMATRIX modelViewProj;
	};

public:
	TransformCBuffer( Graphics& gfx, UINT slot = 0u );
	void InitializeParentReference( const Drawable& parent ) noexcept override;
	std::unique_ptr<CloningBindable> Clone() const noexcept override { return std::make_unique<TransformCBuffer>( *this ); }
	void Bind( Graphics& gfx ) IFNOEXCEPT override { UpdateBind( gfx, GetTransform( gfx ) ); }

protected:
	void UpdateBind( Graphics& gfx, const Transforms &transforms ) noexcept;
	Transforms GetTransform( Graphics & gfx ) const noexcept;

private:
	std::wstring GetUID() const noexcept override;

protected:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVertConstBuffer;
	const Drawable* pParent = nullptr;
};