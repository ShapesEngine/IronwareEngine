/*!
 * \file IronException.h
 * \date 2020/11/11 21:27
 *
 * \author Magic
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
	IronException( int line, const char* file ) noexcept;

	/**
	 * 	overridden function that will return type and formatted string.
	 * 
	 * \return const char* buffer
	 */
	const char* what() const noexcept override;

	/**
	 * 	 *.
	 * 
	 * \return formatted origin std::string 
	 */
	std::string GetOriginString() const noexcept;

public:
	inline const char* GetType() const noexcept { return "Iron Exception"; }
	inline int GetLine() const noexcept { return line; }
	inline const std::string& GetFile() const noexcept { return file; }

private:
	int line;
	std::string file;

protected:
	mutable std::string whatBuffer;
};

