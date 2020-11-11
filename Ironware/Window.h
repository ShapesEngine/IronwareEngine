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
#include "IronException.h"

class Window
{
public:
	/*!
	 * \class Exception
	 *
	 * \brief class to handle window exceptions
	 *
	 * \author Magic
	 * \date November 2020
	 */
	class Exception : public IronException
	{
	public:
		Exception( int line, const char* file, HRESULT hr ) noexcept;
		
		/**
		 * 	overridden function that will return type, error code, description and formatted string.
		 *
		 * \return const char* buffer
		 */
		const char* what() const noexcept override;		
		static std::string TranslateErrorCode( HRESULT hr ) noexcept;

		inline virtual const char* GetType() const noexcept { return "Iron Window Exception"; }
		inline HRESULT GetErrorCode() const noexcept { return hr; }
		inline std::string GetErrorString() const noexcept { return TranslateErrorCode( hr ); }

	private:
		HRESULT hr;
	};

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

// error exception helper macro
#define IRWND_EXCEPT( hr ) Window::Exception( __LINE__, __FILE__, hr )