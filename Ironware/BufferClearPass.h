/*!
 * \file BufferClearPass.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "Pass.h"

class BufferResource;

class BufferClearPass : public Pass
{
public:
	BufferClearPass( std::string name );
	void Execute( Graphics& gfx ) const IFNOEXCEPT override;

private:
	std::shared_ptr<BufferResource> buffer;
};