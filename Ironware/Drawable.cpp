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

void Drawable::Draw( Graphics& gfx ) const IFNOEXCEPT
{
	for( auto& b : binds )
	{
		b->Bind( gfx );
	}
	gfx.DrawIndexed( pIndexBuffer->GetCount() );
}

void Drawable::AddBind( std::shared_ptr<Bindable> bind ) IFNOEXCEPT
{
	if( const auto pi = dynamic_cast<IndexBuffer*>( bind.get() ) )
	{
		pIndexBuffer = pi;
	}
	binds.push_back( std::move( bind ) );
}