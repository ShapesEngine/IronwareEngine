#include "Window.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow )
{
	Window wnd( 640, 480, "Ironware" );

	MSG msg;
	BOOL gResult;
	while( ( gResult = GetMessage( &msg, nullptr, 0, 0 ) ) > 0 )
	{
		// TranslateMessage will post auxiliary WM_CHAR messages from key msgs
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	// check if GetMessage call itself borked
	if( gResult == -1 )
	{
		return -1;
	}

	// wParam here is the value passed to PostQuitMessage
	return msg.wParam;
}