/*!
 * \file Surface.h
 * \date 2020/11/03 11:31
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief In this file we have Surface, Color, Exception classes.
 * * Surface needs Color class to represent color of a pixel.
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "IronException.h"
#include "CommonMacros.h"

#include <string>
#include <cassert>
#include <memory>

/*!
 * \class Surface
 *
 * \brief Surface could be used for setting/getting pixel data,
 * * saving & loading a texture from a file.
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
class Surface
{
public:
	/*!
	 * \class Color
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
		constexpr Color( const Color& col ) noexcept = default;
		constexpr Color( uint32_t dw ) noexcept :
			dword( dw )
		{}
		constexpr Color( uint8_t x, uint8_t r, uint8_t g, uint8_t b ) noexcept :
			dword( ( x << 24u ) | ( r << 16u ) | ( g << 8u ) | b )
		{}
		constexpr Color( uint8_t r, uint8_t g, uint8_t b ) noexcept :
			dword( ( r << 16u ) | ( g << 8u ) | b )
		{}
		constexpr Color( Color col, uint8_t x ) noexcept :
			Color( ( x << 24u ) | col.dword )
		{}
		Color& operator =( Color color ) noexcept
		{
			dword = color.dword;
			return *this;
		}
		constexpr uint8_t GetX() const noexcept { return dword >> 24u; }
		constexpr uint8_t GetA() const noexcept { return GetX(); }
		constexpr uint8_t GetR() const noexcept { return ( dword >> 16u ) & 0xFFu; }
		constexpr uint8_t GetG() const noexcept { return ( dword >> 8u ) & 0xFFu; }
		constexpr uint8_t GetB() const noexcept { return dword & 0xFFu; }
		void SetX( uint8_t x ) noexcept { dword = ( dword & 0xFFFFFFu ) | ( x << 24u ); }
		void SetA( uint8_t a ) noexcept { SetX( a ); }
		void SetR( uint8_t r ) noexcept { dword = ( dword & 0xFF00FFFFu ) | ( r << 16u ); }
		void SetG( uint8_t g ) noexcept { dword = ( dword & 0xFFFF00FFu ) | ( g << 8u ); }
		void SetB( uint8_t b ) noexcept { dword = ( dword & 0xFFFFFF00u ) | b; }
	};

	/*!
	 * \class Exception
	 *
	 * \brief A class for handling surface exceptions
	 *
	 * \author Yernar Aldabergenov
	 * \date March 2021
	 */
	class Exception : public IronException
	{
	public:
		Exception( int line, const wchar_t* file, std::wstring note ) noexcept;
		const char* what() const noexcept override;

		const wchar_t* GetType() const noexcept override { return L"Iron Surface Exception"; }
		const std::wstring& GetNote() const noexcept { return note; }
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

	void PutPixel( uint32_t x, uint32_t y, Color c ) IFNOEXCEPT;
	Color GetPixel( uint32_t x, uint32_t y ) const IFNOEXCEPT;
	static Surface FromFile( const std::wstring& name );
	void Save( const std::wstring& filename ) const;
	void Copy( const Surface& src ) IFNOEXCEPT;

	void Clear( Color fillValue ) noexcept { memset( pBuffer.get(), fillValue.dword, (size_t)width * height * sizeof( Color ) ); }
	uint32_t GetWidth() const noexcept { return width; }
	uint32_t GetHeight() const noexcept { return height; }
	Color* GetBufferPtr() noexcept { return pBuffer.get(); }
	const Color* GetBufferPtr() const noexcept { return pBuffer.get(); }
	const Color* GetBufferPtrConst() const noexcept { return pBuffer.get(); }
	bool IsAlphaLoaded() const noexcept { return alphaLoaded; }

private:
	Surface( uint32_t width, uint32_t height, std::unique_ptr<Color[]> pBufferParam, bool alphaLoaded = false ) noexcept;

private:
	std::unique_ptr<Color[]> pBuffer;
	uint32_t width;
	uint32_t height;
	bool alphaLoaded = false;
};