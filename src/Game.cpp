#include "../inc/Window.h"
#include "../inc/DXCore.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	DXCore dx;
	Window win;
	win.init(1024, 1024, "My Window");
	dx.init(win.width, win.height, win.hwnd, false);
		while (true)
		{
			dx.clear();
			win.processMessages();
			dx.present();
		}
}
