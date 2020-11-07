#include <Windows.h>

int CALLBACK WinMain( 
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nShowCmd 
)
{
	constexpr auto pClassName = "CNIronware";
	constexpr auto pWindowName = "Ironware Engine";

	//////////////////////////////////////////////////////////////////////////
	// Register Windows Class
	// -----------------------------------------------------------------------
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof( wc );
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIcon = nullptr;
	RegisterClassEx( &wc );

	//////////////////////////////////////////////////////////////////////////
	// Create Window Instance
	// -----------------------------------------------------------------------  
	HWND hWnd = CreateWindowEx(
		0, pClassName,
		pWindowName,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		0, 0, 640, 480,
		nullptr, nullptr,
		hInstance, nullptr
	);
	//////////////////////////////////////////////////////////////////////////
	// Show the Window
	// -----------------------------------------------------------------------
	ShowWindow( hWnd, SW_SHOW );
	while( true );
	return 0;
}