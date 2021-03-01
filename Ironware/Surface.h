/*!
 * \file Surface.h
 * \date 2020/11/03 11:31
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief In this file we have Surface, Color, Exception classes.
 * * Surface needs Color class to represent color of a pixel.
 * * Surface could be used for setting/getting pixel data,
 * * saving & loading a texture from a file.  
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "IronException.h"

#include <string>
#include <assert.h>
#include <memory>

class Surface
{
public:
	/*!
	 * \class Surface
	 *
	 * \brief A class to represent the color data
	 *
	 * \author Yernar Aldabergenov
	 * \date November 2020
	 */
	class Color
	{
	public:
		uint32_t dword = 0;

	public:
		constexpr Color() noexcept = default;
		constexpr Color( const Color& col ) noexcept
			:
			dword( col.dword )
		{}
		constexpr Color( uint32_t dw ) noexcept
			:
			dword( dw )
		{}
		constexpr Color( uint8_t x, uint8_t r, uint8_t g, uint8_t b ) noexcept
			:
			dword( ( x << 24u ) | ( r << 16u ) | ( g << 8u ) | b )
		{}
		constexpr Color( uint8_t r, uint8_t g, uint8_t b ) noexcept
			:
			dword( ( r << 16u ) | ( g << 8u ) | b )
		{}
		constexpr Color( Color col, uint8_t x ) noexcept
			:
			Color( ( x << 24u ) | col.dword )
		{}
		Color& operator =( Color color ) noexcept 
		{ 
			dword = color.dword; 	
			return *this; 
		}
		__forceinline constexpr uint8_t GetX() const noexcept { return dword >> 24u; }
		__forceinline constexpr uint8_t GetA() const noexcept { return GetX(); }
		__forceinline constexpr uint8_t GetR() const noexcept { return ( dword >> 16u ) & 0xFFu; }
		__forceinline constexpr uint8_t GetG() const noexcept { return ( dword >> 8u ) & 0xFFu; }
		__forceinline constexpr uint8_t GetB() const noexcept { return dword & 0xFFu; }
		__forceinline void SetX( uint8_t x ) noexcept { dword = ( dword & 0xFFFFFFu ) | ( x << 24u ); }
		__forceinline void SetA( uint8_t a ) noexcept { SetX( a ); }
		__forceinline void SetR( uint8_t r ) noexcept { dword = ( dword & 0xFF00FFFFu ) | ( r << 16u ); }
		__forceinline void SetG( uint8_t g ) noexcept { dword = ( dword & 0xFFFF00FFu ) | ( g << 8u ); }
		__forceinline void SetB( uint8_t b ) noexcept { dword = ( dword & 0xFFFFFF00u ) | b; }
	};

	class Exception : public IronException
	{
	public:
		Exception( int line, const wchar_t* file, std::wstring note ) noexcept;
		const char* what() const noexcept override;

		__forceinline const wchar_t* GetType() const noexcept override { return L"Iron Surface Exception"; }
		__forceinline const std::wstring& GetNote() const noexcept { return note; }
	private:
		std::wstring note;
	};

public:
	Surface( uint32_t width, uint32_t height ) noexcept;
	Surface( Surface&& source ) noexcept;
	Surface( Surface& ) = delete;
	Surface& operator=( Surface&& donor ) noexcept;
	Surface& operator=( const Surface& ) = delete;
	~Surface() = default;
	
	void PutPixel( uint32_t x, uint32_t y, Color c ) noexcept( !IS_DEBUG );
	Color GetPixel( uint32_t x, uint32_t y ) const noexcept( !IS_DEBUG );	
	static Surface FromFile( const std::wstring& name );
	void Save( const std::wstring& filename ) const;
	void Copy( const Surface& src ) noexcept( !IS_DEBUG );

	__forceinline void Clear( Color fillValue ) noexcept { memset( pBuffer.get(), fillValue.dword, (size_t)width * height * sizeof( Color ) ); }
	__forceinline uint32_t GetWidth() const noexcept { return width; }
	__forceinline uint32_t GetHeight() const noexcept { return height; }
	__forceinline Color* GetBufferPtr() noexcept { return pBuffer.get(); }
	__forceinline const Color* GetBufferPtr() const noexcept { return pBuffer.get(); }
	__forceinline const Color* GetBufferPtrConst() const noexcept { return pBuffer.get(); }

private:
	Surface( uint32_t width, uint32_t height, std::unique_ptr<Color[]> pBufferParam ) noexcept;

private:
	std::unique_ptr<Color[]> pBuffer;
	uint32_t width;
	uint32_t height;
};