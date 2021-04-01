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
#include <d3d11.h>

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
	enum class ElementType
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

	class Element
	{
	public:
		Element( ElementType type, size_t offset ) :
			type( type ),
			offset( offset )
		{
			assert( type != ElementType::Count );
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
		__forceinline ElementType GetType() const { return type; }

		static constexpr size_t SizeOf( ElementType type ) noexcept( !IS_DEBUG );

		D3D11_INPUT_ELEMENT_DESC GetDesc() const noexcept( !IS_DEBUG );		

	private:
		template<ElementType type>
		static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDesc( size_t offset ) noexcept( !IS_DEBUG )
		{
			return { Map<type>::semantic, 0, Map<type>::dxgiFormat, 0, (UINT)offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}

	private:
		ElementType type;
		size_t offset;
	};

public:
	/**
	 * @brief Resolves an element in the layout with an appropriate element type
	 * @tparam Type of the element
	 * @return Reference to the element
	*/
	template<ElementType ElementType>
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

	VertexLayout& Append(ElementType ElementType) noexcept( !IS_DEBUG );

	std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const noexcept( !IS_DEBUG );	

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
	template<ElType DestLayoutType, typename SrcType>
	void SetAttribute( std::byte* dest, SrcType&& srcValue ) noexcept( !IS_DEBUG );

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

constexpr size_t VertexLayout::Element::SizeOf( ElementType type ) noexcept( !IS_DEBUG )
{
	using namespace DirectX;
	switch( type )
	{
	case ElementType::Position2D:
		return sizeof( Map<ElementType::Position2D>::SysType );
	case ElementType::Position3D:
		return sizeof( Map<ElementType::Position3D>::SysType );
	case ElementType::Texture2D:
		return sizeof( Map<ElementType::Texture2D>::SysType );
	case ElementType::Normal:
		return sizeof( Map<ElementType::Normal>::SysType );
	case ElementType::Float3Color:
		return sizeof( Map<ElementType::Float3Color>::SysType );
	case ElementType::Float4Color:
		return sizeof( Map<ElementType::Float4Color>::SysType );
	case ElementType::BGRAColor:
		return sizeof( Map<ElementType::BGRAColor>::SysType );
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

#pragma endregion layoutImpl

#pragma region vertexImpl

template<Vertex::ElType Type>
auto& Vertex::Attribute() noexcept( !IS_DEBUG )
{
	auto pAttribute = pData + layout.Resolve<Type>().GetOffset();
	return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>( pAttribute );
}

template<typename T>
void Vertex::SetAttributeByIndex( size_t index, T && value ) noexcept( !IS_DEBUG )
{
	const auto& element = layout.ResolveByIndex( index );
	auto pAttribute = pData + element.GetOffset();
	switch( element.GetType() )
	{
	case ElType::Position2D:
		SetAttribute<ElType::Position2D>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Position3D:
		SetAttribute<ElType::Position3D>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Texture2D:
		SetAttribute<ElType::Texture2D>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Normal:
		SetAttribute<ElType::Normal>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Float3Color:
		SetAttribute<ElType::Float3Color>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::Float4Color:
		SetAttribute<ElType::Float4Color>( pAttribute, std::forward<T>( value ) );
		break;
	case ElType::BGRAColor:
		SetAttribute<ElType::BGRAColor>( pAttribute, std::forward<T>( value ) );
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
	SetAttributeByIndex( index + 1u, std::forward<Rest>( rest )... );
}

template<VertexLayout::ElementType DestLayoutType, typename SrcType>
void Vertex::SetAttribute( std::byte* dest, SrcType&& srcValue ) noexcept( !IS_DEBUG )
{
	using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;
	if constexpr( std::is_assignable<Dest, SrcType>::value )
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
	Back().SetAttributeByIndex( 0u, std::forward<Params>( params )... );
}

#pragma endregion bufferImpl

#pragma endregion impl