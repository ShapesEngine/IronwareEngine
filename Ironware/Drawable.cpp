/*!
 * \class Drawable
 *
 * \ingroup DEV
 *
 * \brief A drawable abstract class that controls(partly) the graphics pipeline and provides
 * * an interface for the other geometry object types
 *
 * TODO:
 *
 * \note It contains: pIndexBuffer, as it's needed to get its count.
 * *				  binds collection that stores various bindable types in it.
 *
 * \author Yernar Aldabergenov
 *
 * \version 1.0
 *
 * \date November 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
#include "Drawable.h"
#include "GraphicsExceptionMacros.h"
#include "IndexBuffer.h"
#include "Bindable.h"

#include <cassert>
#include <typeinfo>

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
	assert( "*Must* use AddIndexBuffer to bind index buffer" && typeid( *bind ) != typeid( IndexBuffer ) );
	binds.push_back( std::move( bind ) );
}

void Drawable::AddIndexBuffer( std::unique_ptr<IndexBuffer> ibuf ) noexcept( !IS_DEBUG )
{
	assert( "Attempting to add index buffer a second time" && pIndexBuffer == nullptr );
	pIndexBuffer = ibuf.get();
	binds.push_back( std::move( ibuf ) );
}
