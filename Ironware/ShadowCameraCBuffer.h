/*!
 * \file ShadowCameraCBuffer.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "Bindable.h"
#include "ConstantBuffers.h"

class Camera;

class ShadowCameraCBuffer : public Bindable
{
protected:
	struct Transform
	{
		DirectX::XMMATRIX ViewProj;
	};

public:
	ShadowCameraCBuffer( Graphics& gfx, UINT slot = 1u );
	void Update( Graphics& gfx );
	void SetCamera( const Camera* pCamera_in ) noexcept;

	void Bind( Graphics& gfx ) IFNOEXCEPT override { pVcbuf->Bind( gfx ); }

private:
	std::unique_ptr<VertexConstantBuffer<Transform>> pVcbuf;
	const Camera* pCamera = nullptr;
};