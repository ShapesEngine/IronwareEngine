/*!
 * \file Mouse.h
 * \date 2020/11/14 7:35
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief A class that is responsible for controlling mouse input
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include <queue>
#include <optional>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPRESS,
			LRELEASE,
			RPRESS,
			RRELEASE,
			WHEELUP,
			WHEELDOWN,
			MOVE,
			ENTER,
			LEAVE,
		};

	private:
		Type type;
		bool leftIsPressed = false;
		bool rightIsPressed = false;
		int x = 0;
		int y = 0;

	public:
		Event() noexcept = default;
		Event( Type type, const Mouse& parent ) noexcept :
			type( type ),
			leftIsPressed( parent.leftIsPressed ),
			rightIsPressed( parent.rightIsPressed ),
			x( parent.x ),
			y( parent.y )
		{}

		inline Type GetType() const noexcept { return type; }
		inline std::pair<int, int> GetPos() const noexcept { return{ x, y }; }
		inline int GetPosX() const noexcept { return x; }
		inline int GetPosY() const noexcept { return y; }
		inline bool LeftIsPressed() const noexcept { return leftIsPressed; }
		inline bool RightIsPressed() const noexcept { return rightIsPressed; }
	};

public:
	Mouse() = default;
	Mouse( const Mouse& ) = delete;
	Mouse& operator=( const Mouse& ) = delete;

	std::optional<Mouse::Event> Read() noexcept;

	inline bool IsInWindow() const noexcept { return isInWindow; }
	inline std::pair<int, int> GetPos() const noexcept { return { x, y }; }
	inline int GetPosX() const noexcept { return x; }
	inline int GetPosY() const noexcept { return y; }
	inline bool LeftIsPressed() const noexcept { return leftIsPressed; }
	inline bool RightIsPressed() const noexcept { return rightIsPressed; }	
	inline bool IsEmpty() const noexcept { return buffer.empty(); }
	inline void Clear() noexcept { buffer = std::queue<Event>(); }

private:
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseMove( int x, int y ) noexcept;
	void OnLeftPressed( int x, int y ) noexcept;
	void OnLeftReleased( int x, int y ) noexcept;
	void OnRightPressed( int x, int y ) noexcept;
	void OnRightReleased( int x, int y ) noexcept;
	void OnWheelUp( int x, int y ) noexcept;
	void OnWheelDown( int x, int y ) noexcept;
	void OnWheelDelta( int x,int y,int delta ) noexcept;
	void TrimBuffer() noexcept;

private:
	static constexpr uint32_t MAXBUFFERSIZE = 16u;
	int x = 0;
	int y = 0;
	int wheelDeltaCarry = 0;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool isInWindow = false;
	std::queue<Event> buffer;
};

