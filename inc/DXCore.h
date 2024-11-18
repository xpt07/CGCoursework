#pragma once
#include "Adapter.h"

class DXCore
{
public:
	ID3D11Device* device;
	ID3D11DeviceContext* devicecontext;
	IDXGISwapChain* swapchain;

	void init(int width, int height, HWND hwnd, bool window_fullscreen);

};