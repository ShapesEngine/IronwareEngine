/*!
 * \class IronException
 *
 * \brief class for handling different kinds of exceptions
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */

#include "IronException.h"

#include <sstream>

IronException::IronException( int line, const wchar_t* file ) noexcept
	:
	line( line ),
	file( file )
{}

const char* IronException::what() const noexcept
{
	std::wostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return reinterpret_cast<const char*>( whatBuffer.c_str() );
}

std::wstring IronException::GetOriginString() const noexcept
{
	std::wostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;

	return oss.str();
}
