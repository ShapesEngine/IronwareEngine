/*!
 * \file Keyboard.cpp
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 *
 * 
 */
#include "Keyboard.h"

std::optional<Keyboard::Event> Keyboard::ReadKey() noexcept
{
	if( keybuffer.size() > 0u )
	{
		Keyboard::Event e = keybuffer.front();
		keybuffer.pop();
		return e;
	}
	return {};
}

std::optional<wchar_t> Keyboard::ReadChar() noexcept
{
	if( charbuffer.size() > 0u )
	{
		wchar_t charcode = charbuffer.front();
		charbuffer.pop();
		return charcode;
	}

	return {};
}

void Keyboard::OnKeyPressed( uint8_t keycode ) noexcept
{
	keystates[keycode] = true;
	keybuffer.push( Keyboard::Event( Keyboard::Event::Type::PRESS, keycode ) );
	TrimBuffer( keybuffer );
}

void Keyboard::OnKeyReleased( uint8_t keycode ) noexcept
{
	keystates[keycode] = false;
	keybuffer.push( Keyboard::Event( Keyboard::Event::Type::RELEASE, keycode ) );
	TrimBuffer( keybuffer );
}

void Keyboard::OnChar( wchar_t character ) noexcept
{
	charbuffer.push( character );
	TrimBuffer( charbuffer );
}

template<typename T>
void Keyboard::TrimBuffer( std::queue<T>& buffer ) noexcept
{
	while( buffer.size() > MAXBUFFERSIZE )
	{
		buffer.pop();
	}
}