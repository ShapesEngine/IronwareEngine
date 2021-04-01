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
#include <dxgiformat.h>
#include <type_traits>

#define TPACK typename...

// ARGB color struct
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
		enum class Type
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

	public:
		Element( Type type, size_t offset ) :
			type( type ),
			offset( offset )
		{
			assert( type != Type::Count );
		}

		/**
		 * @return offset after this element( previous offset + size of this element )
		*/
		__forceinline size_t GetOffsetAfter() const { return offset + GetSize(); }

		__forceinline size_t GetOffset() const { return offset; }
		/**
		 * @return size of the element type
		*/
		__forceinline size_t GetSize() const { return SizeOf( type ); }
		__forceinline Type GetType() const { return type; }

		static constexpr size_t SizeOf( Type type ) noexcept( !IS_DEBUG );

	private:
		Type type;
		size_t offset;
	};
	// alias
	using ElementType = Element::Type;

	template<ElementType> struct Map;
	template<> struct Map<ElementType::Position2D>
	{
		using SysType = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* semantic = "Position";
	};
	template<> struct Map<ElementType::Position3D>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Position";
	};
	template<> struct Map<ElementType::Texture2D>
	{
		using SysType = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* semantic = "TexCoord";
	};
	template<> struct Map<ElementType::Normal>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Normal";
	};
	template<> struct Map<ElementType::Float3Color>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Color";
	};
	template<> struct Map<ElementType::Float4Color>
	{
		using SysType = DirectX::XMFLOAT4;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
		static constexpr const char* semantic = "Color";
	};
	template<> struct Map<ElementType::BGRAColor>
	{
		using SysType = BGRAColor;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		static constexpr const char* semantic = "Color";
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
	__forceinline const Element& ResolveByIndex( size_t index ) const noexcept( !IS_DEBUG ) { return elements[index]; }
	/**
	 * @return size of the elements in bytes
	*/
	__forceinline size_t Size() const noexcept( !IS_DEBUG ) { return elements.empty() ? 0 : elements.back().GetOffsetAfter(); }
	__forceinline size_t GetElementCount() const noexcept( !IS_DEBUG ) { return elements.size(); }

	template<Element::Type Type>
	VertexLayout& Append() noexcept( !IS_DEBUG );

private:
	std::vector<Element> elements;
};

/**
 * @brief Middle level class that works as a view to the vbuffer.
 * * Doesn't hold the buffer itself, but has a pointer to the memory,
 * * where appropriate data is stored.
*/
class Vertex
{
	using ElType = VertexLayout::ElementType;
	friend class VertexByteBuffer;
public:
	/**
	 * @brief Gives an access to the element from the layout.
	 * * Can be used for modification/obtaining the value of the element.
	 * @tparam Semantic attribute type
	 * @return Reference to appropriate memory data. Reinterpreted as needed type.
	*/
	template<ElType Type>
	auto& Attribute() noexcept( !IS_DEBUG );

private:
	Vertex( std::byte* data, const VertexLayout& layout );

	/**
	 * @brief Sets the attribute with appropriate index
	 * @tparam T value type
	 * @param index number in the layout
	 * @param value that will be set
	*/
	template<typename T>
	void SetAttributeByIndex( size_t index, T&& value ) noexcept( !IS_DEBUG );

	/**
	 * @brief Sets the attributes with parameter pack
	 * @tparam First type of the single element in the pack
	 * @tparam Rest pack itself
	 * @param index only needed for parameter pack recursion, default value is 0
	*/
	template<typename First, TPACK Rest>
	void SetAttributeByIndex( size_t index, First&& first, Rest&&... rest ) noexcept( !IS_DEBUG );

	/**
	 * @brief Sets the attribute value
	 * @tparam Dest type of the location
	 * @tparam Src type of the value
	 * @param dest memory address
	 * @param srcValue that will be set
	*/
	template<typename Dest, typename Src>
	void SetAttribute( std::byte* dest, Src&& srcValue ) noexcept( !IS_DEBUG );

private:
	std::byte* pData;
	const VertexLayout& layout;
};

/**
 * @brief Middle level class that works as a view to the vbuffer.
 * * Doesn't hold the buffer itself, but has a pointer to the memory,
 * * where appropriate data is stored.
*/
class ConstVertex
{
public:
	ConstVertex( const Vertex& v ) noexcept( !IS_DEBUG ) :
		vertex( v )
	{}

	template<VertexLayout::ElementType Type>
	__forceinline const auto& Attribute() const noexcept( !IS_DEBUG ) { return const_cast<Vertex&>( vertex ).Attribute<Type>(); }

private:
	Vertex vertex;
};

/**
 * @brief Lower level class that holds data buffer as bytes.
 * * Allows emplacing the vertices to the buffer.
*/
class VertexByteBuffer
{
public:
	VertexByteBuffer( VertexLayout layout );

	__forceinline VertexLayout& GetLayout() noexcept { return layout; }
	__forceinline const VertexLayout& GetLayout() const noexcept { return layout; }
	/**
	 * @return attribute count in the buffer
	*/
	__forceinline size_t Size() const noexcept { return buffer.size() / layout.Size(); }
	/**
	 * @return attribute count in the buffer
	*/
	__forceinline size_t Count() const noexcept { return Size(); }
	__forceinline size_t SizeBytes() const noexcept { return buffer.size(); }
	__forceinline std::byte* GetData() noexcept { return buffer.data(); }
	__forceinline const std::byte* GetData() const noexcept { return const_cast<VertexByteBuffer*>( this )->GetData(); }

	/**
	 * @brief Sets the vertex layout values(order needs to be the as the layout's ordering)
	 * @param ...params values as parameter pack
	*/
	template<TPACK Params>
	void EmplaceBack( Params&&... params ) noexcept( !IS_DEBUG );

	Vertex Back() noexcept( !IS_DEBUG );
	Vertex Front() noexcept( !IS_DEBUG );

	/**
	 * @brief Retrieves vertex with appropriate index
	 * @param index represents the index value with appropriate layout
	 * @return Vertex instance that will hold all the elements of a single vertex
	*/
	Vertex operator[]( size_t index ) noexcept( !IS_DEBUG );

	__forceinline ConstVertex Back() const noexcept( !IS_DEBUG ) { return const_cast<VertexByteBuffer*>( this )->Back(); }
	__forceinline ConstVertex Front() const noexcept( !IS_DEBUG ) { return const_cast<VertexByteBuffer*>( this )->Front(); }
	/**
	 * @brief Retrieves constant vertex with appropriate index
	 * @param index represents the index value with appropriate layout
	 * @return ConstVertex instance that will hold all the elements of a single vertex
	*/
	__forceinline ConstVertex operator[]( size_t index ) const noexcept( !IS_DEBUG ) { return const_cast<VertexByteBuffer&>( *this )[index]; }

private:
	// buffer has no alignment, so when you will be dealing
	// with data you have to keep it in mind
	std::vector<std::byte> buffer;
	VertexLayout layout;
};

#pragma region impl

#pragma region layoutImpl

constexpr size_t VertexLayout::Element::SizeOf( Type type ) noexcept( !IS_DEBUG )
{
	using namespace DirectX;
	switch( type )
	{
	case VertexLayout::ElementType::Position2D:
	case VertexLayout::ElementType::Texture2D:
		return sizeof( XMFLOAT2 );

	case VertexLayout::ElementType::Position3D:
	case VertexLayout::ElementType::Normal:
	case VertexLayout::ElementType::Float3Color:
	case VertexLayout::ElementType::Float4Color:
		return sizeof( XMFLOAT3 );

	case VertexLayout::ElementType::BGRAColor:
		return sizeof( BGRAColor );

	case VertexLayout::ElementType::Count:
		assert( "Don't use count here!" && false );
	}
	assert( "Invalid element type" && false );
	return 0u;
}

template<VertexLayout::ElementType Type>
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

template<VertexLayout::ElementType Type>
VertexLayout& VertexLayout::Append() noexcept( !IS_DEBUG )
{
	static_assert( Type != VertexLayout::ElementType::Count );
	elements.emplace_back( Type, Size() );
	return *this;
}

#pragma endregion layoutImpl

#pragma region vertexImpl

template<Vertex::ElType Type>
auto& Vertex::Attribute() noexcept( !IS_DEBUG )
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
		return *reinterpret_cast<std::byte*>( pAttribute );
		assert( "Don't use count here!" && false );
	}
	else
	{
		return *reinterpret_cast<std::byte*>( pAttribute );
		assert( "Bad element type" && false );
	}
}

template<typename T>
void Vertex::SetAttributeByIndex( size_t index, T && value ) noexcept( !IS_DEBUG )
{
	using namespace DirectX;
	const auto& element = layout.ResolveByIndex( index );
	auto pAttribute = pData + element.GetOffset();
	switch( element.GetType() )
	{
	case ElType::Position2D:
		SetAttribute<XMFLOAT2>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Position3D:
		SetAttribute<XMFLOAT3>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Texture2D:
		SetAttribute<XMFLOAT2>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Normal:
		SetAttribute<XMFLOAT3>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Float3Color:
		SetAttribute<XMFLOAT3>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Float4Color:
		SetAttribute<XMFLOAT4>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::BGRAColor:
		SetAttribute<BGRAColor>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Count:
		assert( "Don't use count type here!" && false );
		break;
	default:
		assert( "Bad element type" && false );
	}
}

template<typename First, TPACK Rest>
void Vertex::SetAttributeByIndex( size_t index, First&& first, Rest&&... rest ) noexcept( !IS_DEBUG )
{
	SetAttributeByIndex( index, std::forward<First>( first ) );
	SetAttributeByIndex( index + 1, std::forward<Rest>( rest )... );
}

template<typename Dest, typename Src>
void Vertex::SetAttribute( std::byte* dest, Src&& srcValue ) noexcept( !IS_DEBUG )
{
	if constexpr( std::is_assignable<Dest, Src>::value )
	{
		*reinterpret_cast<Dest*>( dest ) = srcValue;
	}
	else
	{
		assert( "destination is not assignable" && false );
	}
}

#pragma endregion vertexImpl

#pragma region bufferImpl

template<TPACK Params>
void VertexByteBuffer::EmplaceBack( Params&&... params ) noexcept( !IS_DEBUG )
{
	assert( sizeof...( params ) == layout.GetElementCount() && "Parameter and layout count is NOT equal!" );
	const auto layoutSize = layout.Size();
	buffer.resize( layoutSize + buffer.size() );
	Back().SetAttributeByIndex( 0, std::forward<Params>( params )... );
}

#pragma endregion bufferImpl

#pragma endregion impl