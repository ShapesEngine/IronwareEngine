/*!
 * \file BufferClearPass.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#include "BufferClearPass.h"
#include "RenderTarget.h"
#include "DepthStencilView.h"
#include "Sink.h"
#include "Source.h"

BufferClearPass::BufferClearPass( std::string name ) :
	Pass( std::move( name ) )
{
	RegisterSink( DirectBufferSink<BufferResource>::Make( "buffer", buffer ) );
	RegisterSource( DirectBufferSource<BufferResource>::Make( "buffer", buffer ) );
}

void BufferClearPass::Execute( Graphics& gfx ) const IFNOEXCEPT
{
	buffer->Clear( gfx );
}