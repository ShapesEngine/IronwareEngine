/*!
 * \file Vertex.cpp
 *
 * \author Yernar Aldabergenov
 * \date March 2021
 *
 *
 */
#define DVTX_SOURCE_FILE
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

Vertex VertexByteBuffer::Back() IFNOEXCEPT
{
	assert( !buffer.empty() );
	return Vertex{ buffer.data() + buffer.size() - layout.Size(), layout };
}
Vertex VertexByteBuffer::Front() IFNOEXCEPT
{
	assert( !buffer.empty() );
	return Vertex{ buffer.data(), layout };
}

Vertex VertexByteBuffer::operator[]( size_t index ) IFNOEXCEPT
{
	assert( index < Size() );
	return Vertex{ buffer.data() + index * layout.Size(), layout };
}

VertexLayout& VertexLayout::Append( ElementType Type ) IFNOEXCEPT
{
	assert( Type != ElementType::Count );
	elements.emplace_back( Type, Size() );
	return *this;
}

std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetD3DLayout() const IFNOEXCEPT
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
	desc.reserve( GetElementCount() );
	for( const auto& e : elements )
	{
		desc.push_back( e.GetDesc() );
	}
	return desc;
}

std::wstring VertexLayout::GetCode() const IFNOEXCEPT
{
	std::wstring code;
	for( const auto& e : elements )
	{
		code += e.GetCode();
	}
	return code;
}

template<VertexLayout::ElementType type>
struct DescGenerate
{
	static constexpr D3D11_INPUT_ELEMENT_DESC Exec( size_t offset ) noexcept
	{
		return {
			VertexLayout::Map<type>::semantic,0,
			VertexLayout::Map<type>::dxgiFormat,
			0,(UINT)offset,D3D11_INPUT_PER_VERTEX_DATA,0
		};
	}
};

D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc() const IFNOEXCEPT
{
	return Bridge<DescGenerate>( type, GetOffset() );
}

template<VertexLayout::ElementType type>
struct CodeLookup
{
	static constexpr auto Exec() noexcept
	{
		return VertexLayout::Map<type>::code;
	}
};

const wchar_t* VertexLayout::Element::GetCode() const IFNOEXCEPT
{
	return Bridge<CodeLookup>( type );
}