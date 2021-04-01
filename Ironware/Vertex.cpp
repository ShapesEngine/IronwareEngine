/*!
 * \file Vertex.cpp
 *
 * \author Yernar Aldabergenov
 * \date March 2021
 *
 *
 */
#include "Vertex.h"

Vertex::Vertex( std::byte* data, const VertexLayout& layout ) :
	pData( data ),
	layout( layout )
{
	assert( pData != nullptr );
}

VertexByteBuffer::VertexByteBuffer( VertexLayout layout ) :
	layout( std::move( layout ) )
{}

Vertex VertexByteBuffer::Back() noexcept( !IS_DEBUG )
{
	assert( !buffer.empty() );
	return Vertex{ buffer.data() + buffer.size() - layout.Size(), layout };
}
Vertex VertexByteBuffer::Front() noexcept( !IS_DEBUG )
{
	assert( !buffer.empty() );
	return Vertex{ buffer.data(), layout };
}

Vertex VertexByteBuffer::operator[]( size_t index ) noexcept( !IS_DEBUG )
{
	assert( index < Size() );
	return Vertex{ buffer.data() + index * layout.Size(), layout };
}

VertexLayout & VertexLayout::Append( Element::Type Type ) noexcept( !IS_DEBUG )
{
	assert( Type != VertexLayout::ElementType::Count );
	elements.emplace_back( Type, Size() );
	return *this;
}