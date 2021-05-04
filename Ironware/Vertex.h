/*!
 * \file Vertex.h
 * \date 2021/03/31 18:37
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief Header file that contains VertexLayout, Vertex and VertexByteBuffer
*/
#pragma once

#include "CommonMacros.h"
#include "IronUtils.h"
#include "IronWin.h"

#include <string>
#include <vector>
#include <DirectXMath.h>
#include <dxgiformat.h>
#include <type_traits>
#include <d3d11.h>

#include <assimp/scene.h>

#define DVTX_ELEMENT_AI_EXTRACTOR(member) static SysType Extract( const aiMesh& mesh,size_t i ) noexcept {return *reinterpret_cast<const SysType*>(&mesh.member[i]);}

#define LAYOUT_ELEMENT_TYPES \
	X( Position2D ) \
	X( Position3D ) \
	X( Texture2D ) \
	X( Normal ) \
	X( Tangent ) \
	X( Bitangent ) \
	X( Float3Color ) \
	X( Float4Color ) \
	X( BGRAColor ) \
	X( Count )

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
#define X(el) el,
		LAYOUT_ELEMENT_TYPES
#undef X
	};

	template<ElementType> struct Map;
	template<> struct Map<ElementType::Position2D>
	{
		using SysType = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* semantic = "Position";
		static constexpr const wchar_t* code = L"P2";
		DVTX_ELEMENT_AI_EXTRACTOR( mVertices )
	};
	template<> struct Map<ElementType::Position3D>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Position";
		static constexpr const wchar_t* code = L"P3";
		DVTX_ELEMENT_AI_EXTRACTOR( mVertices )
	};
	template<> struct Map<ElementType::Texture2D>
	{
		using SysType = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* semantic = "TexCoord";
		static constexpr const wchar_t* code = L"T2";
		DVTX_ELEMENT_AI_EXTRACTOR( mTextureCoords[0] )
	};
	template<> struct Map<ElementType::Normal>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Normal";
		static constexpr const wchar_t* code = L"N";
		DVTX_ELEMENT_AI_EXTRACTOR( mNormals )
	};
	template<> struct Map<ElementType::Tangent>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Tangent";
		static constexpr const wchar_t* code = L"T";
		DVTX_ELEMENT_AI_EXTRACTOR( mTangents )
	};
	template<> struct Map<ElementType::Bitangent>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Bitangent";
		static constexpr const wchar_t* code = L"B";
		DVTX_ELEMENT_AI_EXTRACTOR( mBitangents )
	};
	template<> struct Map<ElementType::Float3Color>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Color";
		static constexpr const wchar_t* code = L"CF3";
		DVTX_ELEMENT_AI_EXTRACTOR( mColors[0] )
	};
	template<> struct Map<ElementType::Float4Color>
	{
		using SysType = DirectX::XMFLOAT4;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
		static constexpr const char* semantic = "Color";
		static constexpr const wchar_t* code = L"CF4";
		DVTX_ELEMENT_AI_EXTRACTOR( mColors[0] )
	};
	template<> struct Map<ElementType::BGRAColor>
	{
		using SysType = BGRAColor;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		static constexpr const char* semantic = "Color";
		static constexpr const wchar_t* code = L"CU4";
		DVTX_ELEMENT_AI_EXTRACTOR( mColors[0] )
	};
	template<> struct Map<ElementType::Count>
	{
		using SysType = long double;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_UNKNOWN;
		static constexpr const char* semantic = "!INVALID!";
		static constexpr const wchar_t* code = L"!INV!";
		DVTX_ELEMENT_AI_EXTRACTOR( mFaces )
	};

	template<template<VertexLayout::ElementType> class F, typename... Args>
	static constexpr auto Bridge( VertexLayout::ElementType type, Args&&... args ) IFNOEXCEPT
	{
		switch( type )
		{
#define X(el) case VertexLayout::ElementType::el: return F<VertexLayout::ElementType::el>::Exec( std::forward<Args>( args )... );
			LAYOUT_ELEMENT_TYPES
#undef X
		}
		assert( "Invalid element type" && false );
		return F<VertexLayout::ElementType::Count>::Exec( std::forward<Args>( args )... );
	}

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
		size_t GetOffsetAfter() const { return offset + GetSize(); }

		size_t GetOffset() const { return offset; }
		/**
		 * @return size of the element type
		*/
		size_t GetSize() const { return SizeOf( type ); }
		ElementType GetType() const { return type; }

		static constexpr size_t SizeOf( ElementType type ) IFNOEXCEPT;

		D3D11_INPUT_ELEMENT_DESC GetDesc() const IFNOEXCEPT;
		const wchar_t* GetCode() const IFNOEXCEPT;

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
	const Element& Resolve() const IFNOEXCEPT;

	/**
	 * @brief Resolves an element in the layout by index
	 * @param index of the element in a vector
	 * @return Reference to the element
	*/
	const Element& ResolveByIndex( size_t index ) const IFNOEXCEPT { return elements[index]; }
	/**
	 * @return size of the elements in bytes
	*/
	size_t Size() const IFNOEXCEPT { return elements.empty() ? 0u : elements.back().GetOffsetAfter(); }
	size_t GetElementCount() const IFNOEXCEPT { return elements.size(); }

	VertexLayout& Append( ElementType ElementType ) IFNOEXCEPT;

	std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const IFNOEXCEPT;
	std::wstring GetCode() const IFNOEXCEPT;
	bool Has( ElementType Type ) const noexcept;

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
private:
	template<VertexLayout::ElementType type>
	struct AttributeSetting
	{
		template<typename T>
		static constexpr auto Exec( Vertex* pVertex, std::byte* pAttribute, T&& val ) IFNOEXCEPT
		{
			return pVertex->SetAttribute<type>( pAttribute, std::forward<T>( val ) );
		}
	};

public:
	/**
	 * @brief Gives an access to the element from the layout.
	 * * Can be used for modification/obtaining the value of the element.
	 * @tparam Semantic attribute type
	 * @return Reference to appropriate memory data. Reinterpreted as needed type.
	*/
	template<ElType Type>
	auto& Attribute() IFNOEXCEPT;

private:
	Vertex( std::byte* data, const VertexLayout& layout );

	/**
	 * @brief Sets the attribute with appropriate index
	 * @tparam T value type
	 * @param index number in the layout
	 * @param value that will be set
	*/
	template<typename T>
	void SetAttributeByIndex( size_t index, T&& value ) IFNOEXCEPT;

	/**
	 * @brief Sets the attributes with parameter pack
	 * @tparam First type of the single element in the pack
	 * @tparam Rest pack itself
	 * @param index only needed for parameter pack recursion, default value is 0
	*/
	template<typename First, TPACK Rest>
	void SetAttributeByIndex( size_t index, First&& first, Rest&&... rest ) IFNOEXCEPT;

	/**
	 * @brief Sets the attribute value
	 * @tparam Dest type of the location
	 * @tparam Src type of the value
	 * @param dest memory address
	 * @param srcValue that will be set
	*/
	template<ElType DestLayoutType, typename SrcType>
	void SetAttribute( std::byte* dest, SrcType&& srcValue ) IFNOEXCEPT;

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
	ConstVertex( const Vertex& v ) IFNOEXCEPT :
		vertex( v )
	{}

	template<VertexLayout::ElementType Type>
	const auto& Attribute() const IFNOEXCEPT { return const_cast<Vertex&>( vertex ).Attribute<Type>(); }

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
	VertexByteBuffer( VertexLayout layout, size_t size = 0u ) IFNOEXCEPT;
	VertexByteBuffer( VertexLayout layout, const aiMesh& mesh );

	void Resize( size_t newSize ) IFNOEXCEPT;
	VertexLayout& GetLayout() noexcept { return layout; }
	const VertexLayout& GetLayout() const noexcept { return layout; }
	/**
	 * @return attribute count in the buffer
	*/
	size_t Size() const noexcept { return buffer.size() / layout.Size(); }
	/**
	 * @return attribute count in the buffer
	*/
	size_t Count() const noexcept { return Size(); }
	size_t SizeBytes() const noexcept { return buffer.size(); }
	std::byte* GetData() noexcept { return buffer.data(); }
	const std::byte* GetData() const noexcept { return const_cast<VertexByteBuffer*>( this )->GetData(); }

	/**
	 * @brief Sets the vertex layout values(order needs to be the as the layout's ordering)
	 * @param ...params values as parameter pack
	*/
	template<TPACK Params>
	void EmplaceBack( Params&&... params ) IFNOEXCEPT;

	Vertex Back() IFNOEXCEPT;
	Vertex Front() IFNOEXCEPT;

	/**
	 * @brief Retrieves vertex with appropriate index
	 * @param index represents the index value with appropriate layout
	 * @return Vertex instance that will hold all the elements of a single vertex
	*/
	Vertex operator[]( size_t index ) IFNOEXCEPT;

	ConstVertex Back() const IFNOEXCEPT { return const_cast<VertexByteBuffer*>( this )->Back(); }
	ConstVertex Front() const IFNOEXCEPT { return const_cast<VertexByteBuffer*>( this )->Front(); }
	/**
	 * @brief Retrieves constant vertex with appropriate index
	 * @param index represents the index value with appropriate layout
	 * @return ConstVertex instance that will hold all the elements of a single vertex
	*/
	ConstVertex operator[]( size_t index ) const IFNOEXCEPT { return const_cast<VertexByteBuffer&>( *this )[index]; }

private:
	// buffer has no alignment, so when you will be dealing
	// with data you have to keep it in mind
	std::vector<std::byte> buffer;
	VertexLayout layout;
};

#pragma region impl

#pragma region layoutImpl

template<VertexLayout::ElementType type>
struct SysSizeLookup
{
	static constexpr auto Exec() noexcept
	{
		return sizeof( VertexLayout::Map<type>::SysType );
	}
};

constexpr size_t VertexLayout::Element::SizeOf( ElementType type ) IFNOEXCEPT
{
	return Bridge<SysSizeLookup>( type );
}

template<VertexLayout::ElementType Type>
const VertexLayout::Element& VertexLayout::Resolve() const IFNOEXCEPT
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
auto& Vertex::Attribute() IFNOEXCEPT
{
	auto pAttribute = pData + layout.Resolve<Type>().GetOffset();
	return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>( pAttribute );
}

template<typename T>
void Vertex::SetAttributeByIndex( size_t index, T && value ) IFNOEXCEPT
{
	const auto& element = layout.ResolveByIndex( index );
	auto pAttribute = pData + element.GetOffset();
	VertexLayout::Bridge<AttributeSetting>(
		element.GetType(), this, pAttribute, std::forward<T>( value )
		);
}

template<typename First, TPACK Rest>
void Vertex::SetAttributeByIndex( size_t index, First&& first, Rest&&... rest ) IFNOEXCEPT
{
	SetAttributeByIndex( index, std::forward<First>( first ) );
	SetAttributeByIndex( index + 1u, std::forward<Rest>( rest )... );
}

template<VertexLayout::ElementType DestLayoutType, typename SrcType>
void Vertex::SetAttribute( std::byte* dest, SrcType&& srcValue ) IFNOEXCEPT
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
void VertexByteBuffer::EmplaceBack( Params&&... params ) IFNOEXCEPT
{
	assert( sizeof...( params ) == layout.GetElementCount() && "Parameter and layout count is NOT equal!" );
	const auto layoutSize = layout.Size();
	buffer.resize( layoutSize + buffer.size() );
	Back().SetAttributeByIndex( 0u, std::forward<Params>( params )... );
}

#pragma endregion bufferImpl

#pragma endregion impl

#undef DVTX_ELEMENT_AI_EXTRACTOR
#ifndef DVTX_SOURCE_FILE
#undef LAYOUT_ELEMENT_TYPES
#endif