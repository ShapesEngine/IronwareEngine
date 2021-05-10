/*!
 * \file RenderGraphCompileException.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#include "RenderGraphCompileException.h"
#include "IronUtils.h"

#include <sstream>

RenderGraphCompileException::RenderGraphCompileException( std::string message, int line, const wchar_t * file ) noexcept :
	IronException( line, file ),
	message( std::move( message ) )
{}

const char * RenderGraphCompileException::what() const noexcept
{
	std::ostringstream oss;
	oss << IronException::what() << std::endl;
	oss << "[message]" << std::endl;
	oss << message;
	whatBuffer = to_wide( oss.str() );
	return to_narrow( whatBuffer ).c_str();
}

const wchar_t * RenderGraphCompileException::GetType() const noexcept
{
	return L"Render Graph Compile Exception";
}

const std::string & RenderGraphCompileException::GetMessage() const noexcept
{
	return message;
}
