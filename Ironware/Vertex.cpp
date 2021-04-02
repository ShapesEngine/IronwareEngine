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

VertexLayout& VertexLayout::Append( ElementType Type ) noexcept( !IS_DEBUG )
{
	assert( Type != ElementType::Count );
	elements.emplace_back( Type, Size() );
	return *this;
}

D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc() const noexcept( !IS_DEBUG )
{
	switch( type )
	{
	case ElementType::Position2D:
		return GenerateDesc<ElementType::Position2D>( GetOffset() );
	case ElementType::Position3D:
		return GenerateDesc<ElementType::Position3D>( GetOffset() );
	case ElementType::Texture2D:
		return GenerateDesc<ElementType::Texture2D>( GetOffset() );
	case ElementType::Normal:
		return GenerateDesc<ElementType::Normal>( GetOffset() );
	case ElementType::Float3Color:
		return GenerateDesc<ElementType::Float3Color>( GetOffset() );
	case ElementType::Float4Color:
		return GenerateDesc<ElementType::Float4Color>( GetOffset() );
	case ElementType::BGRAColor:
		return GenerateDesc<ElementType::BGRAColor>( GetOffset() );
	}
	assert( "Invalid element type" && false );
	return { "INVALID", 0u, DXGI_FORMAT_UNKNOWN, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u };
}

std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetD3DLayout() const noexcept( !IS_DEBUG )
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
	desc.reserve( GetElementCount() );
	for( const auto& e : elements )
	{
		desc.push_back( e.GetDesc() );
	}
	return desc;
}