/*!
 * \file Surface.h
 * \date 2020/12/03 11:31
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief 
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include "IronWin.h"
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
	 * \brief 
	 *
	 * \author Yernar Aldabergenov
	 * \date December 2020
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
		inline constexpr uint8_t GetX() const noexcept { return dword >> 24u; }
		inline constexpr uint8_t GetA() const noexcept { return GetX(); }
		inline constexpr uint8_t GetR() const noexcept { return ( dword >> 16u ) & 0xFFu; }
		inline constexpr uint8_t GetG() const noexcept { return ( dword >> 8u ) & 0xFFu; }
		inline constexpr uint8_t GetB() const noexcept { return dword & 0xFFu; }
		inline void SetX( uint8_t x ) noexcept { dword = ( dword & 0xFFFFFFu ) | ( x << 24u ); }
		inline void SetA( uint8_t a ) noexcept { SetX( a ); }
		inline void SetR( uint8_t r ) noexcept { dword = ( dword & 0xFF00FFFFu ) | ( r << 16u ); }
		inline void SetG( uint8_t g ) noexcept { dword = ( dword & 0xFFFF00FFu ) | ( g << 8u ); }
		inline void SetB( uint8_t b ) noexcept { dword = ( dword & 0xFFFFFF00u ) | b; }
	};

	class Exception : public IronException
	{
	public:
		Exception( int line, const wchar_t* file, std::wstring note ) noexcept;
		const char* what() const noexcept override;

		inline const wchar_t* GetType() const noexcept override { return L"Iron Surface Exception"; }
		inline const std::wstring& GetNote() const noexcept { return note; }
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

	inline void Clear( Color fillValue ) noexcept { memset( pBuffer.get(), fillValue.dword, (size_t)width * height * sizeof( Color ) ); }
	inline uint32_t GetWidth() const noexcept { return width; }
	inline uint32_t GetHeight() const noexcept { return height; }
	inline Color* GetBufferPtr() noexcept { return pBuffer.get(); }
	inline const Color* GetBufferPtr() const noexcept { return pBuffer.get(); }
	inline const Color* GetBufferPtrConst() const noexcept { return pBuffer.get(); }

private:
	Surface( uint32_t width, uint32_t height, std::unique_ptr<Color[]> pBufferParam ) noexcept;

private:
	std::unique_ptr<Color[]> pBuffer;
	uint32_t width;
	uint32_t height;
};

