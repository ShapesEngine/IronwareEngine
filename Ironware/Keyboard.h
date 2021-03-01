/*!
 * \file Keyboard.h
 * \date 2020/09/12 18:30
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief Contains Keyboard class
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include <queue>
#include <bitset>
#include <optional>

/*!
 * \class Keyboard
 *
 * \brief A class that is responsible for controlling keyboard input
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 */
class Keyboard
{
	// Mouse gives access to its private members & function
	// as all the message will come from window side
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

	public:
		Event( Type type, uint8_t code ) noexcept :
			type( type ),
			code( code )
		{}

		__forceinline bool IsPress() const noexcept { return type == Type::PRESS; }
		__forceinline bool IsRelease() const noexcept { return type == Type::RELEASE; }
		__forceinline uint8_t GetCode() const noexcept { return code; }

	private:
		Type type;
		uint8_t code = 0u;
	};

public:
	Keyboard() = default;
	Keyboard( const Keyboard& ) = delete;
	Keyboard& operator=( const Keyboard& ) = delete;

	/**
	 * @brief Clears key and char events
	*/
	__forceinline void Clear() noexcept { ClearKey(), ClearChar(); }

	/******************************* KEY EVENTS START ******************************/
	std::optional<Event> ReadKey() noexcept;
	__forceinline bool KeyIsPressed( uint8_t keycode ) const noexcept { return keystates[keycode]; }
	__forceinline bool KeyIsEmpty() const noexcept { return keybuffer.empty(); }
	__forceinline void ClearKey() noexcept { keybuffer = std::queue<Event>(); }
	/******************************* KEY EVENTS END ******************************/

	/******************************* CHAR EVENT START ******************************/
	std::optional<wchar_t> ReadChar() noexcept;
	__forceinline bool CharIsEmpty() const noexcept { return charbuffer.empty(); }
	__forceinline void ClearChar() noexcept { charbuffer = std::queue<wchar_t>(); }
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

	/**
	 * @brief A function that resets keystates
	*/
	__forceinline void ClearState() noexcept { keystates.reset(); }

	/**
	 * @brief Keeps the buffer's size less than specified MAXBUFFERSIZE
	 * @tparam T queue's template type
	 * @param buffer templated queue that will be trimmed
	*/
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