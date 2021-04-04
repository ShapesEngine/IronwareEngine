/*!
 * \file Window.h
 * \date 2020/09/11 16:30
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
#include "imgui/imgui_impl_win32.h"

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
	 * \date September 2020
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

		__forceinline const wchar_t* GetType() const noexcept override { return L"Iron Window Exception"; }
		__forceinline HRESULT GetErrorCode() const noexcept { return hr; }
		__forceinline std::wstring GetErrorDescription() const noexcept { return Exception::TranslateErrorCode( hr ); }

	private:
		HRESULT hr;
	};

	/*!
	 * \class NoGfxException
	 *
	 * \brief Use this class exception to throw no graphix exceptions, as there is no HRESULT associated with no graphics exceptions
	 *
	 *
	 * \author Yernar Aldabergenov
	 * \date September 2020
	 */
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		__forceinline const wchar_t* GetType() const noexcept override { return L"Iron Window [No Graphics] Exception"; }
	};

private:
	/*!
	 * \class WindowClass
	 *
	 * \brief A singleton class that's needed for WINAPI classes(Registering and Unregistering)
	 *
	 * \author Yernar Aldabergenov
	 * \date September 2020
	 */
	class WindowClass
	{
	public:
		__forceinline static const wchar_t* GetName() noexcept { return wndClassName; }
		__forceinline static HINSTANCE GetInstance() noexcept { return wndClass.hInst; }

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

	/**
	 * @brief Function for displaying message box
	 * @param hWnd Window handle
	 * @param lpText Text to be shown in the content of the message box
	 * @param lpCaption Title of the message box
	 * @param uType Type of the message box, ex. error, info, etc.
	*/
	static void ShowMessageBox( HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType );

	/**
	 * @brief Processes Messages for all windows
	 * @return optional int value that is valid only when there was a quit message
	 * * in other situations the value is invalid
	*/
	static std::optional<int> ProcessMessages() noexcept;

	/**
	 * @return Reference to graphics object
	*/
	Graphics& Gfx() const;

	__forceinline uint32_t GetWidth() const { return width; }
	__forceinline uint32_t GetHeight() const { return height; }
	/**
	 * @return Boolean state that is set manually
	*/
	__forceinline bool IsCursorEnabled() const noexcept { return cursorIsEnabled; }
	/**
	 * @return Boolean state that is updated every frame regarding the window
	*/
	__forceinline bool IsCursorShown() const noexcept { return cursorIsShown; }

	void EnableMouseCursor() noexcept;
	void DisableMouseCursor() noexcept;

private:
	static LRESULT CALLBACK HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	static LRESULT CALLBACK HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	LRESULT HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;

	void ShowMouseCursor() noexcept;
	void HideMouseCursor() noexcept;

	__forceinline void EnableImGuiMouse() const noexcept { ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse; }
	__forceinline void DisableImGuiMouse() const noexcept { ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse; }

	__forceinline void FreeCursor() const noexcept { ConfineCursor( false ); }
	void ConfineCursor( bool isMouseConfinedToWindow = true ) const noexcept;

	/**
	 * @brief function that resets the window procedure.
	 * * It's used to prevent various read access violation errors, mouse, keyboard, etc.
	*/
	__forceinline static void ResetWindowProc() { SetWindowLongPtr( GetActiveWindow(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( &DefWindowProc ) ); }

public:
	Keyboard kbd;
	Mouse mouse;

private:
	uint32_t width;
	uint32_t height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
	// state that is set manually
	bool cursorIsEnabled;
	// state that is updated every frame
	bool cursorIsShown;
};