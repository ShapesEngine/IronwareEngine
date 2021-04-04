/*!
 * \file Mouse.h
 * \date 2020/09/14 7:35
 *
 * \author Yernar Aldabergenov
 * Contact: yernar.aa@gmail.com
 *
 * \brief Contains mouse class
 *
 * TODO:
 *
 * \note
*/
#pragma once

#include <queue>
#include <optional>

/*!
 * \class Mouse
 *
 * \ingroup Input
 *
 * \brief A class that is responsible for controlling mouse input
 *
 * TODO:
 *
 * \note
 *
 * \author Yernar Aldabergenov
 *
 * \version 1.0
 *
 * \date September 2020
 *
 * Contact: yernar.aa@gmail.com
 *
 */
class Mouse
{
	// Mouse gives access to its private members & function
	// as all the message will come from window side
	friend class Window;
public:
	/*!
	 * \class Event
	 *
	 * \brief A class that holds mouse event info at the
	 * * time the event has been triggered.
	 *
	 * \author Yernar Aldabergenov
	 * \date February 2020
	 */
	class Event
	{
	public:
		enum class Type
		{
			LPRESS,
			LRELEASE,
			RPRESS,
			RRELEASE,
			MPRESS,
			MRELEASE,
			WHEELUP,
			WHEELDOWN,
			MOVE,
			ENTER,
			LEAVE,
			RAWMOVE
		};

	public:
		Event() noexcept = default;
		Event( Type type, const Mouse& parent ) noexcept;

		__forceinline Type GetType() const noexcept { return type; }

		/**
		 * @brief A function that returns mouse position at the time the event was triggered
		 * @return Pair of { x, y } coordinates of the mouse
		*/
		__forceinline std::pair<int, int> GetPos() const noexcept { return{ x, y }; }

		/**
		 * @brief A function that returns mouse X position at the time the event was triggered
		 * @return X coordinate of the mouse
		*/
		__forceinline int GetPosX() const noexcept { return x; }

		/**
		 * @brief A function that returns mouse Y position at the time the event was triggered
		 * @return Y coordinate of the mouse
		*/
		__forceinline int GetPosY() const noexcept { return y; }

		/**
		 * @brief A function that returns mouse raw delta values at the time the event was triggered
		 * @return Pair of { x, y } coordinates of the mouse
		*/
		__forceinline std::pair<int, int> GetRawDeltas() const noexcept { return{ dx, dy }; }

		/**
		 * @brief A function that returns mouse raw delta X value at the time the event was triggered
		 * @return X delta value
		*/
		__forceinline int GetRawDeltaX() const noexcept { return dx; }

		/**
		 * @brief A function that returns mouse raw delta Y value at the time the event was triggered
		 * @return Y delta value
		*/
		__forceinline int GetRawDeltaY() const noexcept { return dy; }

		/**
		 * @brief A function that return mouse left button press state at the time the event was triggered
		 * @return A boolean value
		*/
		__forceinline bool LeftIsPressed() const noexcept { return leftIsPressed; }

		/**
		 * @brief A function that return mouse right button press state at the time the event was triggered
		 * @return A boolean value
		*/
		__forceinline bool RightIsPressed() const noexcept { return rightIsPressed; }

		/**
		 * @brief A function that return mouse middle button press state at the time the event was triggered
		 * @return A boolean value
		*/
		__forceinline bool MiddleIsPressed() const noexcept { return rightIsPressed; }

		/**
		* @brief A function that checks if any buttons(left, middle, right) were pressed at the time the event was triggered
		* @return A boolean value
		*/
		__forceinline bool AnyButtonIsPressed() const noexcept { return LeftIsPressed() || RightIsPressed() || MiddleIsPressed(); }

	private:
		Type type;
		bool leftIsPressed = false;
		bool rightIsPressed = false;
		bool middleIsPressed = false;
		// window coordinate values
		int x = 0, y = 0;
		// raw delta values
		int dx = 0, dy = 0;
	};

public:
	Mouse() = default;
	Mouse( const Mouse& ) = delete;
	Mouse& operator=( const Mouse& ) = delete;

	std::optional<Event> Read() noexcept;

	__forceinline bool IsInWindow() const noexcept { return isInWindow; }

	/**
	 * @brief A function that returns current mouse position
	 * @return Pair of { x, y } coordinates of the mouse
	*/
	__forceinline std::pair<int, int> GetPos() const noexcept { return { x, y }; }

	/**
	* @brief A function that returns the current mouse X position
	* @return X coordinate of the mouse
	*/
	__forceinline int GetPosX() const noexcept { return x; }

	/**
	* @brief A function that returns the current mouse Y position
	* @return Y coordinate of the mouse
	*/
	__forceinline int GetPosY() const noexcept { return y; }

	/**
	* @brief A function that checks if the left mouse button is being pressed
	* @return A boolean value
	*/
	__forceinline bool LeftIsPressed() const noexcept { return leftIsPressed; }

	/**
	* @brief A function that checks if the right mouse button is being pressed
	* @return A boolean value
	*/
	__forceinline bool RightIsPressed() const noexcept { return rightIsPressed; }

	/**
	* @brief A function that checks if the middle mouse button is being pressed
	* @return A boolean value
	*/
	__forceinline bool MiddleIsPressed() const noexcept { return middleIsPressed; }

	/**
	* @brief A function that checks if any buttons(left, middle, right) are being pressed
	* @return A boolean value
	*/
	__forceinline bool AnyButtonIsPressed() const noexcept { return LeftIsPressed() || RightIsPressed() || MiddleIsPressed(); }

	/**
	* @brief A function that checks if the mouse is being inactive
	* @return A boolean value
	*/
	__forceinline bool IsEmpty() const noexcept { return buffer.empty(); }

	/**
	 * @brief A function that resets the buffer
	*/
	__forceinline void Clear() noexcept { buffer = std::queue<Event>(); }

private:
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseMove( int x, int y ) noexcept;
	void OnRawDeltaMove( int newdx,int newdy ) noexcept;
	void OnLeftPressed( int x, int y ) noexcept;
	void OnLeftReleased( int x, int y ) noexcept;
	void OnRightPressed( int x, int y ) noexcept;
	void OnRightReleased( int x, int y ) noexcept;
	void OnMiddlePressed( int x, int y ) noexcept;
	void OnMiddleReleased( int x, int y ) noexcept;
	void OnWheelUp( int x, int y ) noexcept;
	void OnWheelDown( int x, int y ) noexcept;
	void OnWheelDelta( int x, int y, int delta ) noexcept;
	void TrimBuffer() noexcept;

private:
	static constexpr uint32_t MAXBUFFERSIZE = 16u;
	// window coordinate values
	int x = 0, y = 0;
	// raw delta values
	int dx = 0, dy = 0;
	int wheelDeltaCarry = 0;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool middleIsPressed = false;
	bool isInWindow = false;
	std::queue<Event> buffer;
};