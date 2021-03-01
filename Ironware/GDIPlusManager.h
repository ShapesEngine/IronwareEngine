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
*/
#pragma once

class GDIPlusManager
{
public:
	GDIPlusManager();
	~GDIPlusManager();

private:
	static ULONG_PTR token;
	static int refCount;
};

