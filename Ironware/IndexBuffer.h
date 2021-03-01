/*!
 * \file IndexBuffer.h
 * \date 2020/09/23 5:19
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief An IndexBuffer wrapper class that will be bound in process of graphics pipeline
 * * and store vertex indices in it.
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer( Graphics& gfx, const std::vector<uint16_t>& indices );

	__forceinline void Bind( Graphics& gfx ) noexcept override { GetContext( gfx )->IASetIndexBuffer( pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u ); }
	__forceinline UINT GetCount() const noexcept { return count; }

protected:
	UINT count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};

