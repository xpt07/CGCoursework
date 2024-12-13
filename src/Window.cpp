#include "../inc/Window.h"

Window* window;

#define WINDOW_GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define WINDOW_GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_DESTROY:	// Handle window destruction (e.g., user closes the window).
	{
		PostQuitMessage(0);	// Signal the application to quit.
		exit(0);			// Exit immediately.
		return 0;
	}
	case WM_CLOSE:	// Handle window close requests.
	{
		PostQuitMessage(0);
		exit(0);
		return 0;
	}
	case WM_KEYDOWN:	// Handle key press events.
	{
		window->keys[(unsigned int)wParam] = true;	// Set the key state to "pressed."
		return 0;
	}
	case WM_KEYUP:
	{
		window->keys[(unsigned int)wParam] = false;	// Set the key state to "released."
		return 0;
	}
	case WM_LBUTTONDOWN:	// Handle left mouse button press.
	{
		window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));	// Update mouse position.
		window->mouseButtons[0] = true;	// Mark the left button as pressed.
		return 0;
	}
	case WM_LBUTTONUP:	// Handle left mouse button release.
	{
		window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));	// Update mouse position.
		window->mouseButtons[0] = false;	// Mark the left button as released.
		return 0;
	}
	case WM_MOUSEMOVE:	// Handle mouse movement.
	{
		window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));	// Update mouse position.
		return 0;
	}

	default:	// Handle all other messages with the default Windows procedure.
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void Window::init(int window_width, int window_height, const string window_name, int window_x, int window_y)
{
	WNDCLASSEX wc;
	hinstance = GetModuleHandle(NULL);
	name = window_name;
	width = window_width;
	height = window_height;

	// Define window class properties.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	std::wstring wname = std::wstring(name.begin(), name.end());
	wc.lpszClassName = wname.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, wname.c_str(), wname.c_str(), style, window_x, window_y, width, height, NULL, NULL, hinstance, this);

	memset(keys, 0, 256 * sizeof(char));

	// Set the global window pointer to this instance.
	window = this;
}

void Window::updateMouse(int x, int y)
{
	mousex = x;
	mousey = y;
}

void Window::processMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::centerCursor()
{
	POINT center = { width / 2, height / 2 };	// Calculate the center point.
	ClientToScreen(hwnd, &center);				// Convert to screen coordinates.
	SetCursorPos(center.x, center.y);			// Move the cursor to the center.
}
