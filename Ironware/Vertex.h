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
	 * @return Refence to the element
	*/
	__forceinline const Element& ResolveByIndex( uint32_t index ) const noexcept( !IS_DEBUG ) { return elements[index]; }
	__forceinline size_t Size() const noexcept( !IS_DEBUG ) { return elements.empty() ? 0 : elements.back().GetOffsetAfter(); }

	template<Element::Type Type>
	VertexLayout& Append() noexcept( !IS_DEBUG );

private:
	std::vector<Element> elements;
};

#pragma region implementation

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

#pragma endregion implementation
