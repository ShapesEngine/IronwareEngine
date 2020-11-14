/*!
 * \class Mouse
 *
 * \ingroup DEV
 *
 * \brief A class that is responsible for controlling mouse input
 *
 * TODO:
 *
 * \note 
 *
 * \author Magic
 *
 * \version 1.0
 *
 * \date November 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */

#include "Mouse.h"

Mouse::Event Mouse::Read() noexcept
{
	if( buffer.size() > 0u )
	{
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	else
	{
		return Mouse::Event();
	}
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

void Mouse::TrimBuffer() noexcept
{
	while( buffer.size() > MAXBUFFERSIZE )
	{
		buffer.pop();
	}
}
