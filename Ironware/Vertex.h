/*!
 * \file Vertex.h
 * \date 2021/03/31 18:37
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief
*/
#pragma once

#include <vector>
#include <DirectXMath.h>
#include <type_traits>

#define TPACK typename...

struct BGRAColor
{
	uint8_t a;
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

/**
 * @brief Upper level class that holds info of elements that is stored in vbuffer
*/
class VertexLayout
{
public:
	class Element
	{
	public:
		enum Type
		{
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Float3Color,
			Float4Color,
			BGRAColor,
			Count
		};

	private:
		Element( Type type, size_t offset ) :
			type( type ),
			offset( offset )
		{
			assert( type != Count );
		}

		__forceinline size_t GetOffset() const { return offset; }
		/**
		 * @return size of the element type
		*/
		__forceinline size_t GetSize() const { return SizeOf( type ); }
		__forceinline Type GetType() const { return type; }

		static constexpr size_t SizeOf( Type type ) noexcept( !IS_DEBUG );

	public:
		/**
		 * @return offset after this element( previous offset + size of this element )
		*/
		__forceinline size_t GetOffsetAfter() const { return offset + GetSize(); }

	private:
		Type type;
		size_t offset;
	};

public:
	/**
	 * @brief Resolves an element in the layout with an appropriate element type
	 * @tparam Type of the element
	 * @return Reference to the element
	*/
	template<Element::Type Type>
	const Element& Resolve() const noexcept( !IS_DEBUG );

	/**
	 * @brief Resolves an element in the layout by index
	 * @param index of the element in a vector
	 * @return Reference to the element
	*/
	__forceinline const Element& ResolveByIndex( uint32_t index ) const noexcept( !IS_DEBUG ) { return elements[index]; }
	__forceinline size_t Size() const noexcept( !IS_DEBUG ) { return elements.empty() ? 0 : elements.back().GetOffsetAfter(); }
	__forceinline size_t GetElementCount() const noexcept( !IS_DEBUG ) { return elements.size(); }

	template<Element::Type Type>
	VertexLayout& Append() noexcept( !IS_DEBUG );

private:
	std::vector<Element> elements;
};

class Vertex
{
	using ElType = VertexLayout::Element::Type;
	friend class VertexByteBuffer;
public:
	/**
	 * @brief Gives an access to the element from the layout.
	 * * Can be used for modification/obtaining the value of the element.
	 * @tparam Semantic attribute type
	 * @return Reference to appropriate memory data. Reinterpreted as needed type.
	*/
	template<ElType Type>
	auto& Element() noexcept( !IS_DEBUG );

private:
	Vertex( std::byte* data, const VertexLayout& layout );

	template<typename T>
	void SetElementByIndex( size_t index, T&& value ) noexcept( !IS_DEBUG );

	template<typename First, TPACK Rest>
	void SetElementByIndex( size_t index, First&& first, Rest&&... rest ) noexcept( !IS_DEBUG );

	template<typename Dest, typename Src>
	void SetElement( Dest* dest, Src&& srcValue ) noexcept( !IS_DEBUG );

private:
	std::byte* pData;
	const VertexLayout& layout;
};

class VertexByteBuffer
{
public:
	VertexByteBuffer( VertexLayout layout );

	__forceinline VertexLayout& GetLayout() noexcept { return layout; }
	/**
	 * @return element count in the buffer
	*/
	__forceinline size_t Size() const noexcept { return layout.GetElementCount(); }

	template<TPACK Params>
	void EmplaceBack( Params&&... params ) noexcept( !IS_DEBUG );

	/**
	 * @brief Retrieves vertex with appropriate index
	 * @tparam alignment represents the alignment of the indexing
	 * @param index represents the index value with appropriate alignment
	 * @return Vertex instance
	*/
	template<size_t alignment>
	Vertex operator[]( size_t index ) noexcept( !IS_DEBUG );

private:
	std::vector<std::byte> buffer;
	VertexLayout layout;
};

#pragma region impl

#pragma region layoutImpl

template<VertexLayout::Element::Type Type>
const VertexLayout::Element& VertexLayout::Resolve() const noexcept( !IS_DEBUG )
{
	for( const auto& e : elements )
	{
		if( e.GetType() == Type )
		{
			return e;
		}
	}
	assert( "Element with provided type hasn't been found!" && false );
	return elements.front();
}

template<VertexLayout::Element::Type Type>
VertexLayout& VertexLayout::Append() noexcept( !IS_DEBUG )
{
	static_assert( Type != Element::Type::Count );
	elements.push_back( Type, Size() );
	return *this;
}

#pragma endregion layoutImpl

#pragma region vertexImpl

template<Vertex::ElType Type>
auto& Vertex::Element() noexcept( !IS_DEBUG )
{
	using namespace DirectX;
	const auto& element = layout.Resolve<Type>();
	auto pAttribute = pData + element.GetOffset();
	if constexpr( Type == ElType::Position2D )
	{
		return *reinterpret_cast<XMFLOAT2*>( pAttribute );
	}
	else if constexpr( Type == ElType::Position3D )
	{
		return *reinterpret_cast<XMFLOAT3*>( pAttribute );
	}
	else if constexpr( Type == ElType::Texture2D )
	{
		return *reinterpret_cast<XMFLOAT2*>( pAttribute );
	}
	else if constexpr( Type == ElType::Normal )
	{
		return *reinterpret_cast<XMFLOAT3*>( pAttribute );
	}
	else if constexpr( Type == ElType::Float3Color )
	{
		return *reinterpret_cast<XMFLOAT3*>( pAttribute );
	}
	else if constexpr( Type == ElType::Float4Color )
	{
		return *reinterpret_cast<XMFLOAT4*>( pAttribute );
	}
	else if constexpr( Type == ElType::BGRAColor )
	{
		return *reinterpret_cast<BGRAColor*>( pAttribute );
	}
	else if constexpr( Type == ElType::Count )
	{
		assert( "Don't use count here!" && false );
	}
	else
	{
		assert( "Bad element type" && false );
	}
	return *reinterpret_cast<std::byte*>( pAttribute );
}

template<typename T>
void Vertex::SetElementByIndex( size_t index, T && value ) noexcept( !IS_DEBUG )
{
	using namespace DirectX;
	const auto& element = layout.ResolveByIndex( index );
	auto pAttribute = pData + element.GetOffset();
	switch( element.GetType() )
	{
	case ElType::Position2D:
		SetElement<XMFLOAT2>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Position3D:
		SetElement<XMFLOAT3>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Texture2D:
		SetElement<XMFLOAT2>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Normal:
		SetElement<XMFLOAT3>( pAttribute, std::forward<T>( value ) );
		break;
	case VertexLayout::Float3Color:
		SetElement<XMFLOAT3>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Float4Color:
		SetElement<XMFLOAT4>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::BGRAColor:
		SetElement<BGRAColor>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Count:
		assert( "Don't use count type here!" && false );
		break;
	default:
		assert( "Bad element type" && false );
	}
}

template<typename First, TPACK Rest>
void Vertex::SetElementByIndex( size_t index, First&& first, Rest&&... rest ) noexcept( !IS_DEBUG )
{
	SetElementByIndex( index, std::forward<First>( first ) );
	SetElementByIndex( index + 1, std::forward<Rest>( rest )... );
}

template<typename Dest, typename Src>
void Vertex::SetElement( Dest* dest, Src&& srcValue ) noexcept( !IS_DEBUG )
{
	if constexpr( std::is_assignable( dest, srcValue ) )
	{
		*dest = srcValue;
	}
	else
	{
		static_assert( "destination is not assignable" && false );
	}
}

#pragma endregion vertexImpl

#pragma region bufferImpl

template<TPACK Params>
void VertexByteBuffer::EmplaceBack( Params&&... params ) noexcept( !IS_DEBUG )
{
	const auto layoutSize = layout.Size();
	static_assert( layoutSize != 0 );
	buffer.resize( layoutSize );
	Vertex{ buffer.data(), layout }.SetElementByIndex( 0, std::forward( params )... );
}

template<size_t alignment>
Vertex VertexByteBuffer::operator[]( size_t index ) noexcept( !IS_DEBUG )
{
	const auto layoutSize = layout.Size();
	const auto i = layoutSize * alignment / layoutSize * index;
	static_assert( layoutSize > alignment );
	assert( layoutSize > i );
	return Vertex{ buffer.data() + i, layout };
}

#pragma endregion bufferImpl

#pragma endregion impl