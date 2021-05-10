/*!
 * \file FullScreenPass.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "BindingPass.h"

class IndexBuffer;
class VertexBuffer;
class VertexShader;
class InputLayout;

class FullscreenPass : public BindingPass
{
public:
	FullscreenPass( const std::string name, Graphics& gfx ) IFNOEXCEPT;
	void Execute( Graphics& gfx ) const IFNOEXCEPT override;
};