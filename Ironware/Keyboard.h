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
			INVALID
		};

	private:
		Type type = Type::INVALID;
		unsigned char code = 0u;

	public:
		Event() noexcept = default;
		Event( Type type, unsigned char code ) noexcept :
			type( type ),
			code( code )
		{}

		inline bool IsPress() const noexcept { return type == Type::PRESS; }
		inline bool IsRelease() const noexcept { return type == Type::RELEASE; }
		inline bool IsValid() const noexcept { return type != Type::INVALID; }
		inline unsigned char GetCode() const noexcept { return code; }
	};

public:
	Keyboard() = default;
	Keyboard( const Keyboard& ) = delete;
	Keyboard& operator=( const Keyboard& ) = delete;

	// clears key and char events
	void Clear() noexcept;

	/******************************* KEY EVENTS START ******************************/
	bool KeyIsPressed( unsigned char keycode ) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void ClearKey() noexcept;
	/******************************* KEY EVENTS END ******************************/
	
	/******************************* CHAR EVENT START ******************************/
	wchar_t ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void ClearChar() noexcept;	
	/******************************* CHAR EVENT END ******************************/

	/******************************* AUTOREPEAT START ******************************/
	inline void EnableAutorepeat() noexcept { autorepeatEnabled = true; }
	inline void DisableAutorepeat() noexcept { autorepeatEnabled = false; }
	inline bool AutorepeatIsEnabled() const noexcept { return autorepeatEnabled; }
	/******************************* AUTOREPEAT END ******************************/

private:
	void OnKeyPressed( unsigned char keycode ) noexcept;
	void OnKeyReleased( unsigned char keycode ) noexcept;
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