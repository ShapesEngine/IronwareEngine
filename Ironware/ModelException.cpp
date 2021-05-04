/*!
 * \file ModelException.cpp
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 * 
 */
#include "ModelException.h"

#include <sstream>

ModelException::ModelException( int line, const wchar_t * filename, const std::string & note ) noexcept :
	IronException( line, filename ),
	note( note )
{}

const char* ModelException::what() const noexcept
{
	std::wostringstream woss;
	woss << IronException::what() << std::endl
		<< "[Note] " << GetNote().c_str();
	whatBuffer = woss.str();
	return CON_CHREINT_CAST( whatBuffer.c_str() );
}