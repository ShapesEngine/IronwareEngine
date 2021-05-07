/*!
 * \file FrameExecutor.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "RenderJob.h"
#include "RenderQueue.h"
#include "DepthStencilView.h"
#include "RenderTarget.h"
#include "BlurPack.h"

#include <array>

 /**
  * @brief Class that is responsible for executing according to multiple rendering queues
 */
class FrameExecutor
{
public:
	FrameExecutor( Graphics& gfx );
	void Execute( Graphics& gfx ) IFNOEXCEPT;
	void Reset() noexcept;

	void Accept( RenderJob job, size_t target ) IFNOEXCEPT { rqs[target].Accept( job ); }

private:
	std::array<RenderQueue, 3> rqs;
	DepthStencilView dsv;
	RenderTarget rt1;
	RenderTarget rt2;
	BlurPack blur;
	std::shared_ptr<class VertexBuffer> pVbFull;
	std::shared_ptr<class IndexBuffer> pIbFull;
	std::shared_ptr<class VertexShader> pVsFull;
	std::shared_ptr<class InputLayout> pLayoutFull;
	std::shared_ptr<class Sampler> pSamplerFull;
};