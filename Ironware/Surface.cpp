/*!
 * \class Surface
 *
 * \brief 
 *
 * \author Yernar Aldabergenov
 * \date December 2020
 */
#define FULL_WIN
#include "Surface.h"
#include "IronUtils.h"

#include <sstream>
#include <algorithm>
namespace Gdiplus
{
	// replacing win minmax with stdminmax
	using std::min;
	using std::max;
}
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

/******************************* SURFACE START ******************************/
Surface::Surface( uint32_t width, uint32_t height ) noexcept :
	pBuffer( std::make_unique<Color[]>( (size_t)width * height ) ),
	width( width ),
	height( height )
{}

Surface::Surface( Surface&& source ) noexcept :
	pBuffer( std::move( source.pBuffer ) ),
	width( source.width ),
	height( source.height )
{}

Surface& Surface::operator=( Surface&& donor ) noexcept
{
	pBuffer = std::move( donor.pBuffer );
	width = donor.width;
	height = donor.height;
	return *this;
}

void Surface::PutPixel( uint32_t x, uint32_t y, Color c ) noexcept( !IS_DEBUG )
{
	assert( x >= 0 );
	assert( y >= 0 );
	assert( x < width );
	assert( y < height );
	pBuffer[(size_t)y * width + x] = c;
}

Surface::Color Surface::GetPixel( uint32_t x, uint32_t y ) const noexcept( !IS_DEBUG )
{
	assert( x >= 0 );
	assert( y >= 0 );
	assert( x < width );
	assert( y < height );
	return pBuffer[(size_t)y * width + x];
}

Surface Surface::FromFile( const std::wstring& name )
{
	uint32_t width = 0;
	uint32_t height = 0;
	std::unique_ptr<Color[]> pBuffer;

	{
		Gdiplus::Bitmap bitmap( name.c_str() );
		if( bitmap.GetLastStatus() != Gdiplus::Status::Ok )
		{
			std::wstringstream wss;
			wss << "Loading image [" << name << "]: failed to load.";
			throw Exception( __LINE__, WFILE, wss.str() );
		}

		width = bitmap.GetWidth();
		height = bitmap.GetHeight();
		pBuffer = std::make_unique<Color[]>( (size_t)width * height );

		for( uint32_t y = 0; y < height; y++ )
		{
			for( uint32_t x = 0; x < width; x++ )
			{
				Gdiplus::Color c;
				bitmap.GetPixel( x, y, &c );
				pBuffer[(size_t)y * width + x] = c.GetValue();
			}
		}
	}

	return Surface( width, height, std::move( pBuffer ) );
}

void Surface::Save( const std::wstring& filename ) const
{
	auto GetEncoderClsid = [&filename]( const WCHAR* format, CLSID* pClsid ) -> void
	{
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes

		Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

		Gdiplus::GetImageEncodersSize( &num, &size );
		if( size == 0 )
		{
			std::wstringstream wss;
			wss << "Saving surface to [" << filename << "]: failed to get encoder; size == 0.";
			throw Exception( __LINE__, WFILE, wss.str() );
		}

		pImageCodecInfo = ( Gdiplus::ImageCodecInfo* )( malloc( size ) );
		if( pImageCodecInfo == nullptr )
		{
			std::wstringstream wss;
			wss << "Saving surface to [" << filename << "]: failed to get encoder; failed to allocate memory.";
			throw Exception( __LINE__, WFILE, wss.str() );
		}

		GetImageEncoders( num, size, pImageCodecInfo );

		for( UINT j = 0; j < num; ++j )
		{
			if( wcscmp( pImageCodecInfo[j].MimeType, format ) == 0 )
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free( pImageCodecInfo );
				return;
			}
		}

		free( pImageCodecInfo );
		std::wstringstream wss;
		wss << "Saving surface to [" << filename <<
			"]: failed to get encoder; failed to find matching encoder.";
		throw Exception( __LINE__, WFILE, wss.str() );
	};

	CLSID bmpID;
	GetEncoderClsid( L"image/bmp", &bmpID );

	Gdiplus::Bitmap bitmap( width, height, width * sizeof( Color ), PixelFormat32bppARGB, (BYTE*)pBuffer.get() );
	if( bitmap.Save( filename.c_str(), &bmpID, nullptr ) != Gdiplus::Status::Ok )
	{
		std::wstringstream wss;
		wss << "Saving surface to [" << filename << "]: failed to save.";
		throw Exception( __LINE__, WFILE, wss.str() );
	}
}

void Surface::Copy( const Surface & src ) noexcept( !IS_DEBUG )
{
	assert( width == src.width );
	assert( height == src.height );
	memcpy( pBuffer.get(), src.pBuffer.get(), (size_t)width * height * sizeof( Color ) );
}

Surface::Surface( uint32_t width, uint32_t height, std::unique_ptr<Color[]> pBufferParam ) noexcept :
	pBuffer( std::move( pBufferParam ) ),
	width( width ),
	height( height )
{}
/******************************* SURFACE END ******************************/

/******************************* SURFACE EXCEPTION START ******************************/
Surface::Exception::Exception( int line, const wchar_t* file, std::wstring note ) noexcept :
	IronException( line, file ),
	note( std::move( note ) )
{}

const char* Surface::Exception::what() const noexcept
{
	std::wostringstream woss;
	woss << CON_WCHREINT_CAST( IronException::what() ) << std::endl
		<< "[Note] " << GetNote();
	whatBuffer = woss.str();
	return reinterpret_cast<const char*>( whatBuffer.c_str() );
}
/******************************* SURFACE EXCEPTION END ******************************/