/*!
 * \file RenderGraphCompileException.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#pragma once

#include "IronWin.h"
#include "IronException.h"

#define RGC_EXCEPTION( message ) RenderGraphCompileException( (message), __LINE__, WFILE )

class RenderGraphCompileException : public IronException
{
public:
	RenderGraphCompileException( std::string message, int line, const wchar_t* file ) noexcept;
	const char* what() const noexcept override;
	const wchar_t* GetType() const noexcept override;
	const std::string& GetMessage() const noexcept;

private:
	std::string message;
};