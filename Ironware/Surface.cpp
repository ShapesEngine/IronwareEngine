/*!
 * \file Surface.cpp
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 *
 *
 */
#include "IronWin.h"
#include "Surface.h"
#include "IronUtils.h"
#include "Window.h"

#include <cassert>
#include <sstream>
#include <filesystem>

#pragma region Surface

Surface::Surface( uint32_t width, uint32_t height )
{
	const auto hr = scratchImg.Initialize2D( format, width, height, 1u, 1u );
	if( FAILED( hr ) )
	{
		throw Surface::Exception( __LINE__, WFILE, L"Failed to initialize Scratch Image", hr );
	}
}

void Surface::PutPixel( uint32_t x, uint32_t y, Color c ) IFNOEXCEPT
{
	assert( x >= 0 );
	assert( y >= 0 );
	assert( x < GetWidth() );
	assert( y < GetHeight() );
	auto& imgData = *scratchImg.GetImage( 0u, 0u, 0u );
	reinterpret_cast<Color*>( imgData.pixels[imgData.rowPitch * y] )[x] = c;
}

Surface::Color Surface::GetPixel( uint32_t x, uint32_t y ) const IFNOEXCEPT
{
	assert( x >= 0 );
	assert( y >= 0 );
	assert( x < GetWidth() );
	assert( y < GetHeight() );
	auto& imgData = *scratchImg.GetImage( 0u, 0u, 0u );
	return reinterpret_cast<Color*>( &imgData.pixels[y * imgData.rowPitch] )[x];
}

Surface Surface::FromFile( const std::wstring & name )
{
	DirectX::ScratchImage scratch;
	HRESULT hr = DirectX::LoadFromWICFile( name.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, scratch );

	if( FAILED( hr ) )
	{
		throw Surface::Exception( __LINE__, WFILE, name, L"Failed to load image", hr );
	}

	if( scratch.GetImage( 0u, 0u, 0u )->format != format )
	{
		DirectX::ScratchImage converted;
		hr = DirectX::Convert(
			*scratch.GetImage( 0u, 0u, 0u ),
			format,
			DirectX::TEX_FILTER_DEFAULT,
			DirectX::TEX_THRESHOLD_DEFAULT,
			converted
		);

		if( FAILED( hr ) )
		{
			throw Exception( __LINE__, WFILE, name, L"Failed to convert image", hr );
		}

		return Surface( std::move( converted ) );
	}

	return Surface( std::move( scratch ) );
}

void Surface::Save( const std::wstring & filename ) const
{
	const auto GetCodecID = []( const std::wstring& filename )
	{
		const std::filesystem::path path = filename;
		const auto ext = path.extension().wstring();
		if( ext == L".png" )
		{
			return DirectX::WIC_CODEC_PNG;
		}
		if( ext == L".jpg" )
		{
			return DirectX::WIC_CODEC_JPEG;
		}
		if( ext == L".bmp" )
		{
			return DirectX::WIC_CODEC_BMP;
		}
		throw Exception( __LINE__, WFILE, filename, L"Image format not supported" );
	};

	HRESULT hr = DirectX::SaveToWICFile(
		*scratchImg.GetImage( 0u, 0u, 0u ),
		DirectX::WIC_FLAGS_NONE,
		GetWICCodec( GetCodecID( filename ) ),
		filename.c_str()
	);
	if( FAILED( hr ) )
	{
		throw Surface::Exception( __LINE__, WFILE, filename, L"Failed to save image", hr );
	}
}

void Surface::Clear( Color fillValue ) noexcept
{
	const auto width = GetWidth();
	const auto height = GetHeight();
	auto& imgData = *scratchImg.GetImage( 0u, 0u, 0u );
	for( size_t y = 0u; y < height; y++ )
	{
		auto rowStart = reinterpret_cast<Color*>( imgData.pixels + imgData.rowPitch * y );
		std::fill( rowStart, rowStart + imgData.width, fillValue );
	}
}

Surface::Surface( DirectX::ScratchImage scratchImg ) noexcept :
	scratchImg( std::move( scratchImg ) )
{}

#pragma endregion Surface

#pragma region SurfaceException

Surface::Exception::Exception( int line, const wchar_t* file, const std::wstring & note, std::optional<HRESULT> hr ) noexcept :
	IronException( line, file ),
	note( L"[Note] " + note )
{
	if( hr )
	{
		this->note = L"[Error String] " + Window::Exception::TranslateErrorCode( *hr ) + this->note;
	}
}

Surface::Exception::Exception( int line, const wchar_t* file, std::wstring filename, const std::wstring & note, std::optional<HRESULT> hr ) noexcept :
	IronException( line, file ),
	note( L"[File] " + filename + L" [Note] " + note )
{
	if( hr )
	{
		this->note = L"[Error String] " + Window::Exception::TranslateErrorCode( *hr ) + this->note;
	}
}

const char* Surface::Exception::what() const noexcept
{
	std::wostringstream woss;
	woss << CON_WCHREINT_CAST( IronException::what() ) << std::endl << GetNote();
	whatBuffer = woss.str();
	return reinterpret_cast<const char*>( whatBuffer.c_str() );
}

#pragma endregion SurfaceException