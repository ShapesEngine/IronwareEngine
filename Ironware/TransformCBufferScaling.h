/*!
 * \file TransformCBufferScaling.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "TransformCBuffer.h"
#include "DynamicConstantBuffer.h"

class TransformCBufferScaling : public TransformCBuffer
{
public:
	TransformCBufferScaling( Graphics& gfx, float scale );
	void Accept( TechniqueProbe& probe ) override;
	void Bind( Graphics& gfx ) noexcept override;
	std::unique_ptr<CloningBindable> Clone() const noexcept override;

private:
	static RawLayout MakeLayout();

private:
	Buffer buf;
};