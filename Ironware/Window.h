/*!
 * \file Window.h
 * \date 2020/11/11 16:30
 *
 * \author Magic
 * Contact: yernar.aa@gmail.com
 *
 * \brief class that controls windows and messages
 *
 *
 * \note
*/

#pragma once

#include "IronWin.h"

class Window
{
private:
	/*!
	 * \class singleton WindowClass
	 *
	 * \brief the class that's needed for WINAPI classes(Registering and Unregistering)
	 *
	 * \author Magic
	 * \date November 2020
	 */
	class WindowClass
	{
	public:
		inline static const char* GetName() noexcept { return wndClassName; }
		inline static HINSTANCE GetInstance() noexcept { return wndClass.hInst; }

	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass( const WindowClass& ) = delete;
		WindowClass& operator=( const WindowClass& ) = delete;
		static constexpr const char* wndClassName = "Ironware Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	Window( int width, int height, const char* name ) noexcept;
	~Window();
	Window( const Window& ) = delete;
	Window& operator=( const Window& ) = delete;

private:
	static LRESULT CALLBACK HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	static LRESULT CALLBACK HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	LRESULT HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;

private:
	int width;
	int height;
	HWND hWnd;	
};

