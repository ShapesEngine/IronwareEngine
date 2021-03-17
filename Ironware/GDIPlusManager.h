/*!
 * \file GDIPlusManager.h
 * \date 2020/11/03 11:20
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A class that is responsible for managing GDIplus
 *
 * TODO:
 *
 * \note GDIPlusManager should be initialized to use GDI+ functions
 * * Remember to create at least 1 instance of this class and
 * * keep in mind that destructor uninitializes so you need to have ref to the class
 * * to use the gdi+ stuff.
*/
#pragma once

/*!
 * \class GDIPlusManager
 *
 * \brief A class that is responsible for managing GDIplus
 *
 * \author Yernar Aldabergenov
 * \date November 2020
 */
class GDIPlusManager
{
public:
	/**
	 * @brief Constructs gdi+manager when there are no instances of this class
	*/
	GDIPlusManager();

	/**
	 * @brief Destructs gdi+manager if it was the last remaining instance of the class
	 */
	~GDIPlusManager();

private:
	static ULONG_PTR token;
	static int refCount;
};

