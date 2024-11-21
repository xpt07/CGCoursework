#include "../inc/Window.h"
#include "../inc/Mesh.h"
#include "../inc/Shaders.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	DXCore dx;
	Window win;
	Triangle tri;
	Shaders shader;
	win.init(1024, 1024, "My Window");
	dx.init(win.width, win.height, win.hwnd, false);
	tri.init(dx);
	shader.init("VertexShader.hlsl", "PixelShader.hlsl", dx);
		while (true)
		{
			dx.clear();
			shader.apply(dx);
			tri.draw(dx);
			win.processMessages();
			dx.present();
		}
}
