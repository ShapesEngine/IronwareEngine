/*!
 * \file IronException.h
 * \date 2020/11/11 21:27
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief class for handling different kinds of exceptions
 *
 * TODO: 
 *
 * \note
*/

#pragma once

#include <exception>
#include <string>

class IronException : public std::exception
{
public:
	IronException( int line, const wchar_t* file ) noexcept;

	/**
	 * 	overridden function that will return type and formatted string.
	 * 
	 * \return const char_t* buffer
	 */
	const char* what() const noexcept override;	

	inline virtual const wchar_t* GetType() const noexcept { return L"Iron Base Exception"; }

protected:
	inline int GetLine() const noexcept { return line; }
	inline const std::wstring& GetFile() const noexcept { return file; }
	/**
	 *
	 *
	 * \return formatted origin std::wstring 
	 */
	std::wstring GetOriginString() const noexcept;

private:
	int line;
	std::wstring file;

protected:
	mutable std::wstring whatBuffer;
};

