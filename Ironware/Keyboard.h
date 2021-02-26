/*!
 * \file Keyboard.h
 * \date 2020/11/12 18:30
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A class that is responsible for controlling keyboard input
 *
 * TODO:
 *
 * \note Contains Event class inside and Window is friend class
 * * as we will need to access this class' member private functions from Window
*/
#pragma once

#include <queue>
#include <bitset>
#include <optional>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			PRESS,
			RELEASE,
		};

	private:
		Type type;
		uint8_t code = 0u;

	public:
		Event( Type type, uint8_t code ) noexcept :
			type( type ),
			code( code )
		{}

		__forceinline bool IsPress() const noexcept { return type == Type::PRESS; }
		__forceinline bool IsRelease() const noexcept { return type == Type::RELEASE; }
		__forceinline uint8_t GetCode() const noexcept { return code; }
	};

public:
	Keyboard() = default;
	Keyboard( const Keyboard& ) = delete;
	Keyboard& operator=( const Keyboard& ) = delete;

	// clears key and char events
	__forceinline void Clear() noexcept { ClearKey(), ClearChar(); }

	/******************************* KEY EVENTS START ******************************/
	std::optional<Event> ReadKey() noexcept;
	__forceinline bool KeyIsPressed( uint8_t keycode ) const noexcept { return keystates[keycode]; }
	__forceinline bool KeyIsEmpty() const noexcept { return keybuffer.empty(); }
	__forceinline void ClearKey() noexcept { std::queue<Event>().swap( keybuffer ); }
	/******************************* KEY EVENTS END ******************************/

	/******************************* CHAR EVENT START ******************************/
	std::optional<wchar_t> ReadChar() noexcept;
	__forceinline bool CharIsEmpty() const noexcept { return charbuffer.empty(); }
	__forceinline void ClearChar() noexcept { std::queue<wchar_t>().swap( charbuffer ); }
	/******************************* CHAR EVENT END ******************************/

	/******************************* AUTOREPEAT START ******************************/
	__forceinline void EnableAutorepeat() noexcept { autorepeatEnabled = true; }
	__forceinline void DisableAutorepeat() noexcept { autorepeatEnabled = false; }
	__forceinline bool AutorepeatIsEnabled() const noexcept { return autorepeatEnabled; }
	/******************************* AUTOREPEAT END ******************************/

private:
	void OnKeyPressed( uint8_t keycode ) noexcept;
	void OnKeyReleased( uint8_t keycode ) noexcept;
	void OnChar( wchar_t character ) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer( std::queue<T>& buffer ) noexcept;

private:
	// number of keys
	static constexpr uint32_t NKEYS = 256u;
	// max buffer size
	static constexpr uint32_t MAXBUFFERSIZE = 16u;
	bool autorepeatEnabled = false;
	std::bitset<NKEYS> keystates;
	std::queue<Event> keybuffer;
	std::queue<wchar_t> charbuffer;
};