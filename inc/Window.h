#pragma once
#include <Windows.h>
#include <memory.h>
#include <string>

using namespace std;

class Window
{
public:
	HWND hwnd;
	HINSTANCE hinstance;
	string name;
	int width, height;
	bool keys[256];
	int mousex;
	int mousey;
	bool mouseButtons[3];

	void init(int window_width, int window_height, const string window_name, int window_x = 0, int window_y = 0);
	void updateMouse(int x, int y);
	void processMessages();
};