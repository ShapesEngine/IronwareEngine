/*!
 * \class IronException
 *
 * \brief class for handling different kinds of exceptions
 *
 * \author Magic
 * \date November 2020
 */

#include "IronException.h"

#include <sstream>

IronException::IronException( int line, const char* file ) noexcept
	:
	line( line ),
	file( file )
{}

const char* IronException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

std::string IronException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;

	return oss.str();
}
