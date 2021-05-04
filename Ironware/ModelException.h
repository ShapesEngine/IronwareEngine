/*!
 * \file ModelException.h
 *
 * \author Yernar Aldabergenov
 * \date May 2021
 *
 *
 */
#pragma once

#include "IronException.h"

/**
* @brief Class that manages error checking for the model
*/
class ModelException : public IronException
{
public:
	ModelException( int line, const wchar_t* filename, const std::string& note ) noexcept;
	const wchar_t* GetType() const noexcept override { return type.c_str(); }
	const std::string& GetNote() const noexcept { return note; }
	const char* what() const noexcept override;

private:
	std::string note;
	std::wstring type = L"Ironware Model Exception";
};