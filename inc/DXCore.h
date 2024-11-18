#pragma once
#include "Adapter.h"

class DXCore
{
public:
	ID3D11Device* device;
	ID3D11DeviceContext* devicecontext;
	IDXGISwapChain* swapchain;
	ID3D11RenderTargetView* backbufferRenderTargetView;
	ID3D11Texture2D* backbuffer;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthbuffer;


	void init(int width, int height, HWND hwnd, bool window_fullscreen);
	void clear();
	void present();
};