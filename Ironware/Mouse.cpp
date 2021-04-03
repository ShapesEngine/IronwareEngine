/*!
 * \file Mouse.cpp
 *
 * \author Yernar Aldabergenov
 * \date September 2020
 *
 */

#include "Mouse.h"
#include "IronWin.h"

Mouse::Event::Event( Type type, const Mouse & parent ) noexcept :
	type( type ),
	leftIsPressed( parent.leftIsPressed ),
	rightIsPressed( parent.rightIsPressed ),
	middleIsPressed( parent.middleIsPressed ),
	x( parent.x ),
	y( parent.y )
{}

std::optional<Mouse::Event> Mouse::Read() noexcept
{
	if( !buffer.empty() )
	{
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	return {};
}

void Mouse::ShowCursor() noexcept
{
	cursorIsShown = true;
	// loop until showcursor's value is more/equal than/to 0,
	// as at this point cursor gets shown
	while( ::ShowCursor( TRUE ) < 0 );
}

void Mouse::HideCursor() noexcept
{
	cursorIsShown = false;
	// loop until showcursor's value is less than 0,
	// as at this point cursor gets hidden
	while( ::ShowCursor( FALSE ) >= 0 );
}

void Mouse::OnMouseLeave() noexcept
{
	isInWindow = false;
	buffer.push( Mouse::Event( Mouse::Event::Type::LEAVE, *this ) );
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
	isInWindow = true;
	buffer.push( Mouse::Event( Mouse::Event::Type::ENTER, *this ) );
	TrimBuffer();
}

void Mouse::OnMouseMove( int newx, int newy ) noexcept
{
	x = newx;
	y = newy;

	buffer.push( Mouse::Event( Mouse::Event::Type::MOVE, *this ) );
	TrimBuffer();
}

void Mouse::OnLeftPressed( int x, int y ) noexcept
{
	leftIsPressed = true;

	buffer.push( Mouse::Event( Mouse::Event::Type::LPRESS, *this ) );
	TrimBuffer();
}

void Mouse::OnLeftReleased( int x, int y ) noexcept
{
	leftIsPressed = false;

	buffer.push( Mouse::Event( Mouse::Event::Type::LRELEASE, *this ) );
	TrimBuffer();
}

void Mouse::OnRightPressed( int x, int y ) noexcept
{
	rightIsPressed = true;

	buffer.push( Mouse::Event( Mouse::Event::Type::RPRESS, *this ) );
	TrimBuffer();
}

void Mouse::OnRightReleased( int x, int y ) noexcept
{
	rightIsPressed = false;

	buffer.push( Mouse::Event( Mouse::Event::Type::RRELEASE, *this ) );
	TrimBuffer();
}

void Mouse::OnMiddlePressed( int x, int y ) noexcept
{
	middleIsPressed = true;

	buffer.push( Mouse::Event( Mouse::Event::Type::MPRESS, *this ) );
	TrimBuffer();
}

void Mouse::OnMiddleReleased( int x, int y ) noexcept
{
	middleIsPressed = false;

	buffer.push( Mouse::Event( Mouse::Event::Type::MRELEASE, *this ) );
	TrimBuffer();
}

void Mouse::OnWheelUp( int x, int y ) noexcept
{
	buffer.push( Mouse::Event( Mouse::Event::Type::WHEELUP, *this ) );
	TrimBuffer();
}

void Mouse::OnWheelDown( int x, int y ) noexcept
{
	buffer.push( Mouse::Event( Mouse::Event::Type::WHEELDOWN, *this ) );
	TrimBuffer();
}

void Mouse::OnWheelDelta( int x, int y, int delta ) noexcept
{
	wheelDeltaCarry += delta;
	// generate events for every 120 
	while( wheelDeltaCarry >= WHEEL_DELTA )
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp( x, y );
	}
	while( wheelDeltaCarry <= -WHEEL_DELTA )
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown( x, y );
	}
}

void Mouse::TrimBuffer() noexcept
{
	while( buffer.size() > MAXBUFFERSIZE )
	{
		buffer.pop();
	}
}