#pragma once
#include <Windows.h>
#include <memory.h>
#include <string>
#include "core.h"

using namespace std;

// Window class manages the rendering window and tracks user input.
class Window
{
public:
	HWND hwnd;				// Handle to the window.
	HINSTANCE hinstance;	// Instance handle for the application.
	string name;			// Name of the window.
	int width, height;		// Dimensions of the window.
	bool keys[256];			// Tracks the state of 256 keyboard keys (pressed or released).
	int mousex;				// Current X position of the mouse.
	int mousey;				// Current Y position of the mouse.
	bool mouseButtons[3];	// Tracks the state of mouse buttons (left, middle, right).

	// Initializes the window with given dimensions and name at a specified position.
	void init(int window_width, int window_height, const string window_name, int window_x = 0, int window_y = 0);
	
	// Updates the mouse position.
	void updateMouse(int x, int y);
	
	// Processes all pending Windows messages (e.g., input events).
	void processMessages();
	
	// Centers the cursor within the window.
	void centerCursor();
};