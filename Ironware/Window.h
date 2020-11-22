/*!
 * \file Window.h
 * \date 2020/11/11 16:30
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A class that controls windows and messages
 *
 *
 * \note
*/

#pragma once

#include "IronWin.h"
#include "IronException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

#include <optional>
#include <memory>

class Window
{
public:
	/*!
	 * \class Exception
	 *
	 * \brief class to handle window exceptions
	 *
	 * \author Yernar Aldabergenov
	 * \date November 2020
	 */
	class Exception : public IronException
	{
		using IronException::IronException;

	public:
		static std::wstring TranslateErrorCode( HRESULT hr ) noexcept;
	};

	class HrException : public Exception
	{
	public:
		HrException( int line, const wchar_t* file, HRESULT hr ) noexcept;
		
		/**
		 * 	overridden function that will return type, error code, description and formatted string.
		 *
		 * \return const char_t* buffer
		 */
		const char* what() const noexcept override;		

		inline const wchar_t* GetType() const noexcept override { return L"Iron Window Exception"; }
		inline HRESULT GetErrorCode() const noexcept { return hr; }
		inline std::wstring GetErrorDescription() const noexcept { return Exception::TranslateErrorCode( hr ); }

	private:
		HRESULT hr;
	};

	/*!
	 * \class Window
	 *
	 * \brief Use this class exception to throw no graphix exceptions, as there is no HRESULT associated with no graphics exceptions
	 *
	 *
	 * \author Yernar Aldabergenov
	 * \date November 2020
	 */
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		inline const wchar_t* GetType() const noexcept override { return L"Iron Window [No Graphics] Exception"; }
	};

private:
	/*!
	 * \class singleton WindowClass
	 *
	 * \brief A class that's needed for WINAPI classes(Registering and Unregistering)
	 *
	 * \author Yernar Aldabergenov
	 * \date November 2020
	 */
	class WindowClass
	{
	public:
		inline static const wchar_t* GetName() noexcept { return wndClassName; }
		inline static HINSTANCE GetInstance() noexcept { return wndClass.hInst; }

	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass( const WindowClass& ) = delete;
		WindowClass& operator=( const WindowClass& ) = delete;
		static constexpr const wchar_t* wndClassName = L"Ironware Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	Window( int width, int height, const wchar_t* name );
	~Window();
	Window( const Window& ) = delete;
	Window& operator=( const Window& ) = delete;

	void SetTitle( const std::wstring& title );
	static std::optional<int> ProcessMessages() noexcept;
	Graphics& Gfx() const;

	inline uint32_t GetWidth() const { return width; }
	inline uint32_t GetHeight() const { return height; }

private:
	static LRESULT CALLBACK HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	static LRESULT CALLBACK HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	LRESULT HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;

public:
	Keyboard kbd;
	Mouse mouse;

private:
	uint32_t width;
	uint32_t height;
	HWND hWnd;	
	std::unique_ptr<Graphics> pGfx;
};