/*!
 * \file Drawable.cpp
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 *
 * 
 */
#include "Drawable.h"
#include "GraphicsExceptionMacros.h"
#include "IndexBuffer.h"
#include "Bindable.h"

#include <cassert>

void Drawable::Draw( Graphics& gfx ) const noexcept( !IS_DEBUG )
{
	for( auto& b : binds )
	{
		b->Bind( gfx );
	}
	for( auto& b : GetStaticBinds() )
	{
		b->Bind( gfx );
	}
	gfx.DrawIndexed( pIndexBuffer->GetCount() );
}

void Drawable::AddBind( std::unique_ptr<Bindable> bind ) noexcept( !IS_DEBUG )
{
	// Check if there was an attempt to bind IndexBuffer without AddIndexBufferBind
	assert( "*Must* use AddIndexBufferBind to bind index buffer" && typeid( *bind ) != typeid( IndexBuffer ) );
	binds.push_back( std::move( bind ) );
}

void Drawable::AddIndexBufferBind( std::unique_ptr<IndexBuffer> ibuf ) noexcept( !IS_DEBUG )
{
	assert( "Attempting to add index buffer a second time" && pIndexBuffer == nullptr );
	pIndexBuffer = ibuf.get();
	binds.push_back( std::move( ibuf ) );
}